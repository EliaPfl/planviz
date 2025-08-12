<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios, { AxiosError } from 'axios';
import Swal from 'sweetalert2';
import CausalLegend from './legends/CausalLegend.vue';


const router = useRouter();
const nodes = ref([]);
const edges = ref([]);
const selectedElement = ref(null);
const selectedType = ref('node'); // 'node' or 'edge'
const isLoading = ref(true);
const cy = ref(null);

onMounted(() => {
    isLoading.value = true;
    axios.get('/api/cg')
        .then(response => {
            let colors = generateColors(response.data["metadata"]["num_sccs"]);

            // SCC colors
            Object.values(response.data["elements"]["nodes"]).forEach(el => {
                if (el.data && el.data.scc_id !== undefined) {
                    el.data.color = colors[el.data.scc_id];
                    el.data.fontColor = getContrastColor(colors[el.data.scc_id]);
                }
            });

            // Edge labels
            Object.values(response.data["elements"]["edges"]).forEach(el => {
                if (el.data && el.data.label) {
                    el.data.originalLabel = el.data.label;

                    if (typeof el.data.label === 'object') {
                        if (Array.isArray(el.data.label)) {
                            el.data.label = el.data.label.join(', ');
                        } else {
                            el.data.label = Object.keys(el.data.label).join(', ');
                        }
                    }
                }
            });

            // shape based on goal
            Object.values(response.data["elements"]["nodes"]).forEach(el => {
                if (el.data.hasOwnProperty('goal')) {
                    el.data.shape = 'round-octagon';
                } else {
                    el.data.shape = 'roundrectangle';
                }
            });

            // cytoscape instance
            cy.value = cytoscape({
                container: document.getElementById('cy'),
                elements: response.data.elements,

                style: [
                    {
                        selector: 'node',
                        style: {
                            'shape': 'data(shape)',
                            'label': 'data(name)',
                            'text-wrap': 'wrap',
                            'text-max-width': 80,
                            'text-valign': 'center',
                            'text-halign': 'center',
                            'padding': '10px',
                            'background-color': 'data(color)',
                            'color': 'data(fontColor)',
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
                            'target-arrow-scale': 3,
                            'curve-style': 'bezier',
                        }
                    },
                    {
                        selector: '.highlighted',
                        style: {
                            'line-color': '#e74c3c',
                            'target-arrow-color': '#e74c3c',
                            'transition-property': 'background-color, line-color, target-arrow-color',
                            'transition-duration': '0.1s',
                            'border-color': '#000',
                            'border-width': '2px',
                            'z-index': 10,
                        }
                    }
                ],

                layout: {
                    name: 'breadthfirst',
                    fit: true,
                    padding: 50,
                    animate: false,
                }
            });

            nodes.value = cy.value.nodes().map(node => node.data());
            edges.value = cy.value.edges().map(edge => edge.data());

            cy.value.on('tap', 'node', handleNodeClick);
            cy.value.on('tap', 'edge', handleEdgeClick);
            cy.value.on('tap', function (evt) {
                if (evt.target === evt.cy) {
                    cy.value.elements().removeClass('highlighted');
                }
            });
            cy.value.on('dblclick', 'node', function (evt) {
                const node = evt.target;
                const nodeId = node.data('id');

                router.push({ name: 'DomainTransitionGraph', params: { ID: nodeId } });
            });
            isLoading.value = false;
            console.log(isLoading.value);
        })
        .catch(error => {
            if (error instanceof AxiosError) {
                if (error.response && error.response.status === 405) {
                    Swal.fire({
                        icon: 'error',
                        title: 'No Causal Graph Available',
                        text: 'Please upload PDDL files first to generate a Causal Graph.',
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

    cy.value.elements().removeClass('highlighted');
    node.addClass('highlighted');
    node.connectedEdges().addClass('highlighted');

    // Scroll to top of the sidebar to show details
    const rightSidebar = document.getElementById('right');
    if (rightSidebar) {
        const scrollContainer = rightSidebar.querySelector('.overflow-y-auto');
        if (scrollContainer) {
            scrollContainer.scrollTo({ top: 0, behavior: 'smooth' });
        }
    }
}

function handleEdgeClick(event) {
    const edge = event.target;
    const edgeData = edge.data();
    selectedElement.value = edgeData;
    selectedType.value = 'edge';

    cy.value.elements().removeClass('highlighted');
    edge.addClass('highlighted');

    // Scroll to top of the sidebar to show details
    const rightSidebar = document.getElementById('right');
    if (rightSidebar) {
        const scrollContainer = rightSidebar.querySelector('.overflow-y-auto');
        if (scrollContainer) {
            scrollContainer.scrollTo({ top: 0, behavior: 'smooth' });
        }
    }
}

function handleListClick(element, type = 'node') {
    selectedElement.value = element;
    selectedType.value = type;

    if (cy.value) {
        cy.value.elements().removeClass('highlighted');

        if (type === 'node') {
            const cyNode = cy.value.getElementById(element.id);
            cyNode.addClass('highlighted');
            cyNode.connectedEdges().addClass('highlighted');
        } else if (type === 'edge') {
            const cyEdge = cy.value.getElementById(element.id);
            cyEdge.addClass('highlighted');
        }
    }

    // Scroll to top of the sidebar to show details
    const rightSidebar = document.getElementById('right');
    if (rightSidebar) {
        const scrollContainer = rightSidebar.querySelector('.overflow-y-auto');
        if (scrollContainer) {
            scrollContainer.scrollTo({ top: 0, behavior: 'smooth' });
        }
    }
}

function getNodeName(nodeId) {
    const node = nodes.value.find(n => n.id === nodeId);
    return node ? node.name : `Node ${nodeId}`;
}

function generateColors(count) {
    const colors = [];
    for (let i = 0; i < count; i++) {
        const hue = (i * 360 / count) % 360;
        const saturation = 70 + (i % 3) * 10;
        const lightness = 50 + (i % 2) * 10;
        colors.push(`hsl(${hue}, ${saturation}%, ${lightness}%)`);
    }
    return colors;
}

function getContrastColor(hslColor) {
    return `rgb(0, 0, 0)`;
}
</script>

<template>
    <div class="flex h-[calc(100vh-4rem)] mt-16 dark:bg-gray-700">
        <div v-if="isLoading"
            class="fixed inset-0 bg-white dark:bg-gray-900 bg-opacity-75 dark:bg-opacity-75 flex items-center justify-center z-999">
            <div class="text-center">
                <div class="inline-block animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mb-4"></div>
                <p class="text-gray-600 dark:text-gray-300 text-lg">Loading Causal Graph...</p>
            </div>
        </div>

        <div id="left" class="w-2/3 p-4 relative">
            <div id="cy" class="w-full h-full bg-gray-100 dark:bg-gray-800 rounded-lg shadow-inner"></div>
            <CausalLegend />
        </div>

        <div id="right" class="w-1/3 p-4">
            <div class="bg-gray-50 dark:bg-gray-800 h-full overflow-y-auto rounded-lg shadow-inner p-4">
                <div class="mb-4">
                    <div class="flex space-x-2 mb-4">
                        <button @click="selectedType = 'node'; selectedElement = null"
                            :class="['px-3 py-1 rounded text-sm', selectedType === 'node' ? 'bg-blue-500 text-white' : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300']">
                            Nodes
                        </button>
                        <button @click="selectedType = 'edge'; selectedElement = null"
                            :class="['px-3 py-1 rounded text-sm', selectedType === 'edge' ? 'bg-blue-500 text-white' : 'bg-gray-200 dark:bg-gray-700 text-gray-700 dark:text-gray-300']">
                            Edges
                        </button>
                    </div>
                </div>

                <!-- Selected Element Details -->
                <div v-if="selectedElement && selectedType === 'node'"
                    class="mb-6 p-4 bg-blue-50 dark:bg-blue-900 rounded-lg border-l-4 border-blue-500">
                    <h3 class="text-lg font-semibold mb-2 text-blue-800 dark:text-blue-300">Selected Node</h3>
                    <div class="space-y-2">
                        <p class="text-gray-900 dark:text-gray-100"><strong>ID:</strong> {{ selectedElement.id }}</p>
                        <p class="text-gray-900 dark:text-gray-100"><strong>Name:</strong> {{ selectedElement.name }}
                        </p>
                        <p v-if="selectedElement.scc_id !== undefined" class="text-gray-900 dark:text-gray-100">
                            <strong>SCC ID:</strong> {{ selectedElement.scc_id }}
                        </p>
                    </div>
                </div>

                <div v-if="selectedElement && selectedType === 'edge'"
                    class="mb-6 p-4 bg-green-50 dark:bg-green-900 rounded-lg border-l-4 border-green-500">
                    <h3 class="text-lg font-semibold mb-2 text-green-800 dark:text-green-300">Selected Edge</h3>
                    <div class="space-y-3">
                        <p class="text-gray-900 dark:text-gray-100"><strong>ID:</strong> {{ selectedElement.id }}</p>
                        <p class="text-gray-900 dark:text-gray-100"><strong>From:</strong> {{
                            getNodeName(selectedElement.source) }}</p>
                        <p class="text-gray-900 dark:text-gray-100"><strong>To:</strong> {{
                            getNodeName(selectedElement.target) }}</p>
                        <div v-if="selectedElement.originalLabel">
                            <strong class="text-gray-900 dark:text-gray-100">Actions:</strong>
                            <div class="mt-2 space-y-2">
                                <div v-for="(params, action) in selectedElement.originalLabel" :key="action"
                                    class="bg-white dark:bg-gray-700 rounded border dark:border-gray-600 p-3">
                                    <div class="font-semibold text-green-700 dark:text-green-300 mb-2 capitalize">{{
                                        action }}</div>
                                    <div class="space-y-1">
                                        <div v-for="(param, index) in params" :key="index"
                                            class="text-sm bg-gray-100 dark:bg-gray-600 rounded px-2 py-1 font-mono text-gray-900 dark:text-gray-100">
                                            ({{ param }})
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div v-else-if="selectedElement.label">
                            <strong class="text-gray-900 dark:text-gray-100">Label:</strong>
                            <div
                                class="mt-1 p-2 bg-white dark:bg-gray-700 rounded border dark:border-gray-600 text-sm text-gray-900 dark:text-gray-100">
                                {{ selectedElement.label }}
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Nodes List -->
                <div v-show="selectedType === 'node'">
                    <h2 class="text-lg font-semibold mb-4 text-gray-900 dark:text-gray-100">Node Details</h2>
                    <ul class="space-y-2">
                        <li @click="handleListClick(node, 'node')" v-for="node in nodes" :key="node.id"
                            :id="`node-` + node.id"
                            :class="['p-2 rounded hover:bg-blue-50 dark:hover:bg-blue-900 cursor-pointer', selectedElement?.id === node.id && selectedType === 'node' ? 'bg-blue-100 dark:bg-blue-900/30 border border-blue-300 dark:border-blue-600' : '']">
                            <strong class="text-gray-900 dark:text-gray-100">{{ node.name }}</strong>
                            <p v-if="node.beschreibung" class="text-sm text-gray-600 dark:text-gray-400">
                                {{ node.beschreibung }}
                            </p>
                            <span
                                class="text-xs bg-blue-100 dark:bg-blue-900/50 text-blue-800 dark:text-blue-300 px-2 py-1 rounded">
                                SCC {{ node.scc_id }}
                            </span>
                        </li>
                    </ul>
                </div>

                <!-- Edges List -->
                <div v-show="selectedType === 'edge'">
                    <h2 class="text-lg font-semibold mb-4 text-gray-900 dark:text-gray-100">All Edges</h2>
                    <ul class="space-y-2">
                        <li v-for="edge in edges" :key="edge.id" :id="`edge-` + edge.id"
                            @click="handleListClick(edge, 'edge')"
                            :class="['p-2 rounded hover:bg-green-50 dark:hover:bg-green-900 cursor-pointer', selectedElement?.id === edge.id && selectedType === 'edge' ? 'bg-green-100 dark:bg-green-900/30 border border-green-300 dark:border-green-600' : '']">
                            <div class="text-sm">
                                <strong class="text-gray-900 dark:text-gray-100">{{ getNodeName(edge.source) }} → {{
                                    getNodeName(edge.target) }}</strong>
                                <p v-if="edge.label" class="text-gray-600 dark:text-gray-400 mt-1">
                                    {{ edge.label }}
                                </p>
                            </div>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>
</template>

<style scoped></style>