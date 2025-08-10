<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios, { AxiosError } from 'axios';
import Swal from 'sweetalert2';
import CausalLegend from './legends/CausalLegend.vue';


const router = useRouter();
const nodes = ref([]);
const elements = ref([]);
const isLoading = ref(true);
const error = ref(null);
const cy = ref(null);
const expandedNodes = ref(new Set());

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
                el.data.actions = extractActionsForNode(el.data.id, response.data.elements.edges);
            });

            // shape based on goal
            Object.values(response.data["elements"]["nodes"]).forEach(el => {
                if (el.data.hasOwnProperty('goal')) {
                    el.data.shape = 'round-octagon';
                } else {
                    el.data.shape = 'roundrectangle';
                }
            });

            elements.value = response.data.elements;

            cy.value = cytoscape({
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

            cy.value.on('tap', 'node', handleNodeClick);
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
    const nodeElement = document.getElementById(`node-${nodeData.id}`);

    cy.value.elements().removeClass('highlighted');
    node.addClass('highlighted');
    node.connectedEdges().addClass('highlighted');

    if (nodeElement) {
        nodeElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
        nodeElement.classList.add('node-highlight');
        nodeElement.addEventListener('animationend', () => {
            nodeElement.classList.remove('node-highlight');
        }, { once: true });
        expandedNodes.value.clear();
        expandedNodes.value.add(nodeData.id);
    }
}

function handleListClick(node) {
    expandedNodes.value.clear();
    if (expandedNodes.value.has(node.id)) {
        expandedNodes.value.delete(node.id);
    } else {
        expandedNodes.value.add(node.id);
    }

    if (cy.value) {
        cy.value.elements().removeClass('highlighted');
        const cyNode = cy.value.getElementById(node.id);
        cyNode.addClass('highlighted');
        cyNode.connectedEdges().addClass('highlighted');
    }
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
    return `rgb(0, 0, 0)`;
}

function extractActionsForNode(nodeId, edges) {
    const actions = new Set();

    edges.forEach(edge => {
        // Wenn Node als Source oder Target beteiligt ist
        if (edge.data.source === nodeId || edge.data.target === nodeId) {
            if (edge.data.label && typeof edge.data.label === 'object') {
                // Alle Aktionstypen aus dem Label extrahieren
                Object.keys(edge.data.label).forEach(actionType => {
                    actions.add(actionType);
                });
            }
        }
    });

    return Array.from(actions);
}
</script>

<template>
    <div class="flex h-[calc(100vh-4rem)] mt-16">
        <div v-if="isLoading" class="fixed inset-0 bg-white bg-opacity-75 flex items-center justify-center z-999">
            <div class="text-center">
                <div class="inline-block animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mb-4"></div>
                <p class="text-gray-600 text-lg">Loading Causal Graph...</p>
            </div>
        </div>

        <div id="left" class="w-2/3 p-4 relative">
            <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
            <CausalLegend />
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
                        <span class="text-xs bg-blue-100 text-blue-800 px-2 py-1 rounded">
                            SCC {{ node.scc_id }}
                        </span>
                        <div v-if="expandedNodes.has(node.id)" class="mt-2">
                            <strong>Actions:</strong>
                            <ul class="list-disc pl-5">
                                <li v-for="action in node.actions" :key="action" class="text-sm text-gray-700">
                                    {{ action }}
                                </li>
                            </ul>
                        </div>
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