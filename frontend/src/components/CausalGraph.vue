<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios from 'axios';

const router = useRouter();
const nodes = ref([]);
const elements = ref([]);
const isLoading = ref(true);
const error = ref(null);

onMounted(() => {
    isLoading.value = true;
    axios.get('/api/cg')
        .then(response => {
            let colors = generateColors(response.data["metadata"]["num_sccs"]);

            Object.values(response.data["elements"]["nodes"]).forEach(el => {
                console.log(el);
                if (el.data && el.data.scc_id !== undefined) {
                    el.data.color = colors[el.data.scc_id];
                    el.data.fontColor = getContrastColor(colors[el.data.scc_id]);
                }
            });

            Object.values(response.data["elements"]["nodes"]).forEach(el => {
                if (el.data.hasOwnProperty('goal')) {
                    el.data.shape = 'round-octagon';
                } else {
                    el.data.shape = 'roundrectangle';
                }
            });
            elements.value = response.data.elements;
            const cy = cytoscape({
                container: document.getElementById('cy'),
                elements: elements.value,

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
                            'curve-style': 'bezier',
                        }
                    }
                ],

                layout: {
                    name: 'grid',
                    fit: true,
                    padding: 50,
                    nodeRepulsion: 10000,
                    edgeElasticity: 1000,
                }
            });

            nodes.value = cy.nodes().map(node => node.data());
            cy.on('tap', 'node', handleNodeClick);
            cy.on('dblclick', 'node', function (evt) {
                const node = evt.target;
                const nodeId = node.data('id');

                router.push({ name: 'DomainTransitionGraph', params: { ID: nodeId } });
            });
            isLoading.value = false;
            console.log(isLoading.value);
        })
        .catch(error => {
            console.error('Error fetching elements:', error);
            error.value = 'Failed to load Causal Graph. Please try again later.';
            isLoading.value = false; f
        });

});

function handleNodeClick(event) {
    const node = event.target;
    const nodeData = node.data();
    const nodeElement = document.getElementById(`node-${nodeData.id}`);

    if (nodeElement) {
        nodeElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
        nodeElement.classList.add('node-highlight');
        nodeElement.addEventListener('animationend', () => {
            nodeElement.classList.remove('node-highlight');
        }, { once: true });
    }
}

function handleListClick(node) {
    router.push({ name: 'DomainTransitionGraph', params: { ID: node.id } });
}

function generateColors(count) {
    const colors = [];
    for (let i = 0; i < count; i++) {
        const hue = (i * 360 / count) % 360;    // Gleichmäßig verteilte Farbtöne
        const saturation = 70 + (i % 3) * 10;   // Variiert zwischen 70-90%
        const lightness = 50 + (i % 2) * 10;    // Variiert zwischen 50-60%
        colors.push(`hsl(${hue}, ${saturation}%, ${lightness}%)`);
    }
    return colors;
}

function getContrastColor(hslColor) {
    return `rgb(#000)`;
}
</script>

<template>
    <div class="flex h-[calc(100vh-4rem)] mt-16">
        <div v-if="isLoading" class="fixed inset-0 bg-white bg-opacity-75 flex items-center justify-center z-0">
            <div class="text-center">
                <div class="inline-block animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mb-4"></div>
                <p class="text-gray-600 text-lg">Loading Causal Graph...</p>
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

        <div id="left" class="w-2/3 p-4">
            <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
        </div>

        <div id="right" class="w-1/3 p-4">
            <div class="bg-gray-50 h-full overflow-y-auto rounded-lg shadow-inner p-4">
                <h2 class="text-lg font-semibold mb-4">Node Details</h2>
                <ul class="space-y-2">
                    <li @click="handleListClick(node)" v-for="node in nodes" :key="node.id" :id="`node-` + node.id"
                        class="p-2 rounded hover:bg-blue-50 cursor-pointer">
                        <strong>{{ node.name }}</strong>
                        <p v-if="node.beschreibung" class="text-sm text-gray-600">
                            {{ node.beschreibung }}
                        </p>
                    </li>
                </ul>
            </div>
        </div>
    </div>
</template>

<style scoped>
@keyframes pulse {

    0%,
    100% {
        background-color: #DBEAFE;
        border-color: #BFDBFE;
    }

    50% {
        background-color: transparent;
        border-color: transparent;
    }
}

.node-highlight {
    animation: pulse 0.6s ease-in-out 0s 2;
    border: 2px solid transparent;
}
</style>