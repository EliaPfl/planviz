<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios from 'axios';

const router = useRouter();
const nodes = ref([]);
const elements = ref([]);

onMounted(() => {

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
        })
        .catch(error => {
            console.error('Error fetching elements:', error);
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
    <div id="left" class="fixed top-16 left-0 w-2/3 h-[calc(100vh-4rem)] p-4">
        <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
    </div>

    <div id="right" class="fixed top-16 right-0 w-1/3 h-[calc(100vh-4rem)] p-4
           bg-gray-100 rounded-lg shadow-inner overflow-y-auto">
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