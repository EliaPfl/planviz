<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios, { AxiosError } from 'axios';
import Swal from 'sweetalert2';
import LandmarkLegend from './legends/LandmarkLegend.vue';

const router = useRouter();
const nodes = ref([]);
const edges = ref([]);
const elements = ref([]);
const selectedElement = ref(null);
const selectedType = ref('node');
const isLoading = ref(true);
const error = ref(null);

onMounted(() => {
    isLoading.value = true;
    axios.get('/api/landmark')
        .then(response => {
            let data = response.data;

            Object.values(data["elements"]["edges"]).forEach(el => {
                switch (el.data.type) {
                    case 0:
                        el.data.line_style = 'dotted';
                        break;
                    case 1:
                        el.data.line_style = 'dashed';
                        break;
                    case 2:
                        el.data.line_style = 'solid';
                        break;
                    case 3:
                        el.data.line_style = 'solid';
                        break;
                }
            });

            const cy = cytoscape({
                container: document.getElementById('cy'),

                elements: data["elements"],

                style: [
                    {
                        selector: 'node',
                        style: {
                            'shape': 'ellipse',
                            'label': 'data(name)',
                            'text-wrap': 'wrap',
                            'text-max-width': 80,
                            'text-valign': 'center',
                            'text-halign': 'center',
                            'padding': '15px',
                            'background-color': 'green',
                            'color': '#fff',
                            'font-size': 12,
                            'width': 'label',
                            'height': 'label',
                        }
                    },
                    {
                        selector: 'edge',
                        style: {
                            'width': 2,
                            'line-color': '#ccc',
                            'target-arrow-color': '#ccc',
                            'target-arrow-shape': 'triangle',
                            'curve-style': 'bezier',
                            'line-style': 'data(line_style)',
                        }
                    }
                ],

                layout: {
                    name: 'breadthfirst',
                    fit: true,
                    padding: 50,
                }
            });

            nodes.value = cy.nodes().map(node => node.data());
            edges.value = cy.edges().map(edge => edge.data());
            cy.on('tap', 'node', handleNodeClick);

            isLoading.value = false;
            console.log(isLoading.value);
        })
        .catch(error => {
            if (error instanceof AxiosError) {
                if (error.response && error.response.status === 405) {
                    Swal.fire({
                        icon: 'error',
                        title: 'No Landmark Graph Available',
                        text: 'Please upload PDDL files first to generate a Landmark Graph.',
                        showCancelButton: true,
                        confirmButtonText: 'Upload Files',
                        confirmButtonColor: '#3B82F6',
                        cancelButtonText: 'Stay Here',
                        cancelButtonColor: '#6B7280',
                    }).then((result) => {
                        if (result.isConfirmed) {
                            router.push('/upload');
                        }
                    });
                    isLoading.value = false;
                    return;
                }
            }
            console.error('Error fetching elements:', error);
            error.value = 'Failed to load Causal Graph. Please try again later.';
            isLoading.value = false;
        });

});

function handleNodeClick(event) {
    const node = event.target;
    const nodeData = node.data();
    selectedElement.value = nodeData;
    selectedType.value = 'node';
    
    // Scroll to top of the sidebar to show details
    const rightSidebar = document.getElementById('right');
    if (rightSidebar) {
        const scrollContainer = rightSidebar.querySelector('.overflow-y-auto');
        if (scrollContainer) {
            scrollContainer.scrollTo({ top: 0, behavior: 'smooth' });
        }
    }
}

</script>

<template>
    <div class="flex h-[calc(100vh-4rem)] mt-16">
        <div v-if="isLoading" class="fixed inset-0 bg-white bg-opacity-75 flex items-center justify-center z-0">
            <div class="text-center">
                <div class="inline-block animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mb-4"></div>
                <p class="text-gray-600 text-lg">Loading Landmark Graph...</p>
            </div>
        </div>

        <div v-else-if="error" class="flex items-center justify-center w-full">
            <div class="text-center p-8">
                <div class="text-red-500 text-6xl mb-4">⚠️</div>
                <h2 class="text-xl font-semibold text-gray-800 mb-2">Error Loading Graph</h2>
                <p class="text-gray-600 mb-4">{{ error }}</p>
                <button @click="window.location.reload()"
                    class="px-4 py-2 bg-blue-600 text-white rounded hover:bg-blue-700">
                    Retry
                </button>
            </div>
        </div>

        <div id="left" class="w-2/3 p-4 relative">
            <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
            <LandmarkLegend />
        </div>

        <div id="right" class="w-1/3 p-4">
            <div class="bg-gray-50 h-full overflow-y-auto rounded-lg shadow-inner p-4">
                <div class="mb-4">
                    <div class="flex space-x-2 mb-4">
                        <button 
                            class="px-3 py-1 rounded text-sm bg-blue-500 text-white">
                            Nodes
                        </button>
                    </div>
                </div>

                <!-- Selected Element Details -->
                <div v-if="selectedElement" class="mb-6 p-4 bg-blue-50 rounded-lg border-l-4 border-blue-500">
                    <h3 class="text-lg font-semibold mb-2 text-blue-800">Selected Node</h3>
                    <div class="space-y-2">
                        <p><strong>ID:</strong> {{ selectedElement.id }}</p>
                        <p><strong>Name:</strong> {{ selectedElement.name }}</p>
                        <p v-if="selectedElement.beschreibung"><strong>Description:</strong> {{ selectedElement.beschreibung }}</p>
                    </div>
                </div>

                <!-- Nodes List -->
                <div>
                    <h2 class="text-lg font-semibold mb-4">All Nodes</h2>
                    <ul class="space-y-2">
                        <li v-for="node in nodes" :key="node.id" :id="`node-` + node.id"
                            @click="selectedElement = node"
                            :class="['p-2 rounded hover:bg-blue-50 cursor-pointer', selectedElement?.id === node.id ? 'bg-blue-100 border border-blue-300' : '']">
                            <strong>{{ node.name }}</strong>
                            <p v-if="node.beschreibung" class="text-sm text-gray-600">
                                {{ node.beschreibung }}
                            </p>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>
</template>

<style scoped>
</style>