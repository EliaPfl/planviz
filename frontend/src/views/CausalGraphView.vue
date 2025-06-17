<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';

const router = useRouter();
const nodes = ref([]);

onMounted(() => {

    const cy = cytoscape({
        container: document.getElementById('cy'),

        elements: [{ "data": { "id": "0", "name": "Atom holding(b)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "1", "name": "Atom clear(b)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "2", "name": "Atom clear(a)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "3", "name": "Atom clear(c)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "4", "name": "Atom clear(d)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "5", "name": "Atom clear(e)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "6", "name": "Atom clear(f)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "7", "name": "Atom clear(g)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "8", "name": "Atom clear(h)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "9", "name": "Atom handempty()", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "10", "name": "Atom holding(a)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "11", "name": "Atom holding(c)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "12", "name": "Atom holding(d)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "13", "name": "Atom holding(e)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "14", "name": "Atom holding(f)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "15", "name": "Atom holding(g)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "16", "name": "Atom holding(h)", beschreibung: "mehr Infos ..." } },
        { "data": { "id": "0_1", "source": "0", "target": "1" } },
        { "data": { "id": "0_2", "source": "0", "target": "2" } },
        { "data": { "id": "0_3", "source": "0", "target": "3" } },
        { "data": { "id": "0_4", "source": "0", "target": "4" } },
        { "data": { "id": "0_5", "source": "0", "target": "5" } },
        { "data": { "id": "0_6", "source": "0", "target": "6" } },
        { "data": { "id": "0_7", "source": "0", "target": "7" } },
        { "data": { "id": "0_8", "source": "0", "target": "8" } },
        { "data": { "id": "0_9", "source": "0", "target": "9" } },
        { "data": { "id": "1_0", "source": "1", "target": "0" } },
        { "data": { "id": "1_2", "source": "1", "target": "2" } },
        { "data": { "id": "1_3", "source": "1", "target": "3" } },
        { "data": { "id": "1_4", "source": "1", "target": "4" } },
        { "data": { "id": "1_5", "source": "1", "target": "5" } },
        { "data": { "id": "1_6", "source": "1", "target": "6" } },
        { "data": { "id": "1_7", "source": "1", "target": "7" } },
        { "data": { "id": "1_8", "source": "1", "target": "8" } },
        { "data": { "id": "1_9", "source": "1", "target": "9" } },
        { "data": { "id": "1_10", "source": "1", "target": "10" } },
        { "data": { "id": "1_11", "source": "1", "target": "11" } },
        { "data": { "id": "1_12", "source": "1", "target": "12" } },
        { "data": { "id": "1_13", "source": "1", "target": "13" } },
        { "data": { "id": "1_14", "source": "1", "target": "14" } },
        { "data": { "id": "1_15", "source": "1", "target": "15" } },
        { "data": { "id": "1_16", "source": "1", "target": "16" } },
        { "data": { "id": "2_0", "source": "2", "target": "0" } },
        { "data": { "id": "2_1", "source": "2", "target": "1" } },
        { "data": { "id": "2_3", "source": "2", "target": "3" } },
        { "data": { "id": "2_4", "source": "2", "target": "4" } },
        { "data": { "id": "2_5", "source": "2", "target": "5" } },
        { "data": { "id": "2_6", "source": "2", "target": "6" } },
        { "data": { "id": "2_7", "source": "2", "target": "7" } },
        { "data": { "id": "2_8", "source": "2", "target": "8" } },
        { "data": { "id": "2_9", "source": "2", "target": "9" } },
        { "data": { "id": "2_10", "source": "2", "target": "10" } },
        { "data": { "id": "2_11", "source": "2", "target": "11" } },
        { "data": { "id": "2_12", "source": "2", "target": "12" } },
        { "data": { "id": "2_13", "source": "2", "target": "13" } },
        { "data": { "id": "2_14", "source": "2", "target": "14" } },
        { "data": { "id": "2_15", "source": "2", "target": "15" } },
        { "data": { "id": "2_16", "source": "2", "target": "16" } },
        { "data": { "id": "3_0", "source": "3", "target": "0" } },
        { "data": { "id": "3_1", "source": "3", "target": "1" } },
        { "data": { "id": "3_2", "source": "3", "target": "2" } },
        { "data": { "id": "3_4", "source": "3", "target": "4" } },
        { "data": { "id": "3_5", "source": "3", "target": "5" } },
        { "data": { "id": "3_6", "source": "3", "target": "6" } },
        { "data": { "id": "3_7", "source": "3", "target": "7" } },
        { "data": { "id": "3_8", "source": "3", "target": "8" } },
        { "data": { "id": "3_9", "source": "3", "target": "9" } },
        { "data": { "id": "3_10", "source": "3", "target": "10" } },
        { "data": { "id": "3_11", "source": "3", "target": "11" } },
        { "data": { "id": "3_12", "source": "3", "target": "12" } },
        { "data": { "id": "3_13", "source": "3", "target": "13" } },
        { "data": { "id": "3_14", "source": "3", "target": "14" } },
        { "data": { "id": "3_15", "source": "3", "target": "15" } },
        { "data": { "id": "3_16", "source": "3", "target": "16" } },
        { "data": { "id": "4_0", "source": "4", "target": "0" } },
        { "data": { "id": "4_1", "source": "4", "target": "1" } },
        { "data": { "id": "4_2", "source": "4", "target": "2" } },
        { "data": { "id": "4_3", "source": "4", "target": "3" } },
        { "data": { "id": "4_5", "source": "4", "target": "5" } },
        { "data": { "id": "4_6", "source": "4", "target": "6" } },
        { "data": { "id": "4_7", "source": "4", "target": "7" } },
        { "data": { "id": "4_8", "source": "4", "target": "8" } },
        { "data": { "id": "4_9", "source": "4", "target": "9" } },
        { "data": { "id": "4_10", "source": "4", "target": "10" } },
        { "data": { "id": "4_11", "source": "4", "target": "11" } },
        { "data": { "id": "4_12", "source": "4", "target": "12" } },
        { "data": { "id": "4_13", "source": "4", "target": "13" } },
        { "data": { "id": "4_14", "source": "4", "target": "14" } },
        { "data": { "id": "4_15", "source": "4", "target": "15" } },
        { "data": { "id": "4_16", "source": "4", "target": "16" } },
        { "data": { "id": "5_0", "source": "5", "target": "0" } },
        { "data": { "id": "5_1", "source": "5", "target": "1" } },
        { "data": { "id": "5_2", "source": "5", "target": "2" } },
        { "data": { "id": "5_3", "source": "5", "target": "3" } },
        { "data": { "id": "5_4", "source": "5", "target": "4" } },
        { "data": { "id": "5_6", "source": "5", "target": "6" } },
        { "data": { "id": "5_7", "source": "5", "target": "7" } },
        { "data": { "id": "5_8", "source": "5", "target": "8" } },
        { "data": { "id": "5_9", "source": "5", "target": "9" } },
        { "data": { "id": "5_10", "source": "5", "target": "10" } },
        { "data": { "id": "5_11", "source": "5", "target": "11" } },
        { "data": { "id": "5_12", "source": "5", "target": "12" } },
        { "data": { "id": "5_13", "source": "5", "target": "13" } },
        { "data": { "id": "5_14", "source": "5", "target": "14" } },
        { "data": { "id": "5_15", "source": "5", "target": "15" } },
        { "data": { "id": "5_16", "source": "5", "target": "16" } },
        { "data": { "id": "6_0", "source": "6", "target": "0" } },
        { "data": { "id": "6_1", "source": "6", "target": "1" } },
        { "data": { "id": "6_2", "source": "6", "target": "2" } },
        { "data": { "id": "6_3", "source": "6", "target": "3" } },
        { "data": { "id": "6_4", "source": "6", "target": "4" } },
        { "data": { "id": "6_5", "source": "6", "target": "5" } },
        { "data": { "id": "6_7", "source": "6", "target": "7" } },
        { "data": { "id": "6_8", "source": "6", "target": "8" } },
        { "data": { "id": "6_9", "source": "6", "target": "9" } },
        { "data": { "id": "6_10", "source": "6", "target": "10" } },
        { "data": { "id": "6_11", "source": "6", "target": "11" } },
        { "data": { "id": "6_12", "source": "6", "target": "12" } },
        { "data": { "id": "6_13", "source": "6", "target": "13" } },
        { "data": { "id": "6_14", "source": "6", "target": "14" } },
        { "data": { "id": "6_15", "source": "6", "target": "15" } },
        { "data": { "id": "6_16", "source": "6", "target": "16" } },
        { "data": { "id": "7_0", "source": "7", "target": "0" } },
        { "data": { "id": "7_1", "source": "7", "target": "1" } },
        { "data": { "id": "7_2", "source": "7", "target": "2" } },
        { "data": { "id": "7_3", "source": "7", "target": "3" } },
        { "data": { "id": "7_4", "source": "7", "target": "4" } },
        { "data": { "id": "7_5", "source": "7", "target": "5" } },
        { "data": { "id": "7_6", "source": "7", "target": "6" } },
        { "data": { "id": "7_8", "source": "7", "target": "8" } },
        { "data": { "id": "7_9", "source": "7", "target": "9" } },
        { "data": { "id": "7_10", "source": "7", "target": "10" } },
        { "data": { "id": "7_11", "source": "7", "target": "11" } },
        { "data": { "id": "7_12", "source": "7", "target": "12" } },
        { "data": { "id": "7_13", "source": "7", "target": "13" } },
        { "data": { "id": "7_14", "source": "7", "target": "14" } },
        { "data": { "id": "7_15", "source": "7", "target": "15" } },
        { "data": { "id": "7_16", "source": "7", "target": "16" } },
        { "data": { "id": "8_0", "source": "8", "target": "0" } },
        { "data": { "id": "8_1", "source": "8", "target": "1" } },
        { "data": { "id": "8_2", "source": "8", "target": "2" } },

        { "data": { "id": "8_4", "source": "8", "target": "4" } },
        { "data": { "id": "8_5", "source": "8", "target": "5" } },
        { "data": { "id": "8_6", "source": "8", "target": "6" } },
        { "data": { "id": "8_7", "source": "8", "target": "7" } },
        { "data": { "id": "8_9", "source": "8", "target": "9" } },
        { "data": { "id": "8_10", "source": "8", "target": "10" } },
        { "data": { "id": "8_11", "source": "8", "target": "11" } },
        { "data": { "id": "8_12", "source": "8", "target": "12" } },
        { "data": { "id": "8_13", "source": "8", "target": "13" } },
        { "data": { "id": "8_14", "source": "8", "target": "14" } },
        { "data": { "id": "8_15", "source": "8", "target": "15" } },
        { "data": { "id": "8_16", "source": "8", "target": "16" } },
        { "data": { "id": "9_0", "source": "9", "target": "0" } },
        { "data": { "id": "9_1", "source": "9", "target": "1" } },
        { "data": { "id": "9_2", "source": "9", "target": "2" } },
        { "data": { "id": "9_3", "source": "9", "target": "3" } },
        { "data": { "id": "9_4", "source": "9", "target": "4" } },
        { "data": { "id": "9_5", "source": "9", "target": "5" } },
        { "data": { "id": "9_6", "source": "9", "target": "6" } },
        { "data": { "id": "9_7", "source": "9", "target": "7" } },
        { "data": { "id": "9_8", "source": "9", "target": "8" } },
        { "data": { "id": "9_10", "source": "9", "target": "10" } },
        { "data": { "id": "9_11", "source": "9", "target": "11" } },
        { "data": { "id": "9_12", "source": "9", "target": "12" } },
        { "data": { "id": "9_13", "source": "9", "target": "13" } },
        { "data": { "id": "9_14", "source": "9", "target": "14" } },
        { "data": { "id": "9_15", "source": "9", "target": "15" } },
        { "data": { "id": "9_16", "source": "9", "target": "16" } },
        { "data": { "id": "10_1", "source": "10", "target": "1" } },
        { "data": { "id": "10_2", "source": "10", "target": "2" } },
        { "data": { "id": "10_3", "source": "10", "target": "3" } },
        { "data": { "id": "10_4", "source": "10", "target": "4" } },
        { "data": { "id": "10_5", "source": "10", "target": "5" } },
        { "data": { "id": "10_6", "source": "10", "target": "6" } },
        { "data": { "id": "10_7", "source": "10", "target": "7" } },
        { "data": { "id": "10_8", "source": "10", "target": "8" } },
        { "data": { "id": "10_9", "source": "10", "target": "9" } },
        { "data": { "id": "11_1", "source": "11", "target": "1" } },
        { "data": { "id": "11_2", "source": "11", "target": "2" } },
        { "data": { "id": "11_3", "source": "11", "target": "3" } },
        { "data": { "id": "11_4", "source": "11", "target": "4" } },
        { "data": { "id": "11_5", "source": "11", "target": "5" } },
        { "data": { "id": "11_6", "source": "11", "target": "6" } },
        { "data": { "id": "11_7", "source": "11", "target": "7" } },
        { "data": { "id": "11_8", "source": "11", "target": "8" } },
        { "data": { "id": "11_9", "source": "11", "target": "9" } },
        { "data": { "id": "12_1", "source": "12", "target": "1" } },
        { "data": { "id": "12_2", "source": "12", "target": "2" } },
        { "data": { "id": "12_3", "source": "12", "target": "3" } },
        { "data": { "id": "12_4", "source": "12", "target": "4" } },
        { "data": { "id": "12_5", "source": "12", "target": "5" } },
        { "data": { "id": "12_6", "source": "12", "target": "6" } },
        { "data": { "id": "12_7", "source": "12", "target": "7" } },
        { "data": { "id": "12_8", "source": "12", "target": "8" } },
        { "data": { "id": "12_9", "source": "12", "target": "9" } },
        { "data": { "id": "13_1", "source": "13", "target": "1" } },
        { "data": { "id": "13_2", "source": "13", "target": "2" } },
        { "data": { "id": "13_3", "source": "13", "target": "3" } },
        { "data": { "id": "13_4", "source": "13", "target": "4" } },
        { "data": { "id": "13_5", "source": "13", "target": "5" } },
        { "data": { "id": "13_6", "source": "13", "target": "6" } },
        { "data": { "id": "13_7", "source": "13", "target": "7" } },
        { "data": { "id": "13_8", "source": "13", "target": "8" } },
        { "data": { "id": "13_9", "source": "13", "target": "9" } },
        { "data": { "id": "14_1", "source": "14", "target": "1" } },
        { "data": { "id": "14_2", "source": "14", "target": "2" } },
        { "data": { "id": "14_3", "source": "14", "target": "3" } },
        { "data": { "id": "14_4", "source": "14", "target": "4" } },
        { "data": { "id": "14_5", "source": "14", "target": "5" } },
        { "data": { "id": "14_6", "source": "14", "target": "6" } },
        { "data": { "id": "14_7", "source": "14", "target": "7" } },
        { "data": { "id": "14_8", "source": "14", "target": "8" } },
        { "data": { "id": "14_9", "source": "14", "target": "9" } },
        { "data": { "id": "15_1", "source": "15", "target": "1" } },
        { "data": { "id": "15_2", "source": "15", "target": "2" } },
        { "data": { "id": "15_3", "source": "15", "target": "3" } },
        { "data": { "id": "15_4", "source": "15", "target": "4" } },
        { "data": { "id": "15_5", "source": "15", "target": "5" } },
        { "data": { "id": "15_6", "source": "15", "target": "6" } },
        { "data": { "id": "15_7", "source": "15", "target": "7" } },
        { "data": { "id": "15_8", "source": "15", "target": "8" } },
        { "data": { "id": "15_9", "source": "15", "target": "9" } },
        { "data": { "id": "16_1", "source": "16", "target": "1" } },
        { "data": { "id": "16_2", "source": "16", "target": "2" } },
        { "data": { "id": "16_3", "source": "16", "target": "3" } },
        { "data": { "id": "16_4", "source": "16", "target": "4" } },
        { "data": { "id": "16_5", "source": "16", "target": "5" } },
        { "data": { "id": "16_6", "source": "16", "target": "6" } },
        { "data": { "id": "16_7", "source": "16", "target": "7" } },
        { "data": { "id": "16_8", "source": "16", "target": "8" } },
        { "data": { "id": "16_9", "source": "16", "target": "9" } },
        ]



        ,

        style: [
            {
                selector: 'node',
                style: {
                    'shape': 'roundrectangle',
                    'label': 'data(name)',
                    'text-wrap': 'wrap',
                    'text-max-width': 80,
                    'text-valign': 'center',
                    'text-halign': 'center',
                    'padding': '10px',
                    'background-color': '#0074D9',
                    'color': '#fff',
                    'font-size': 12,
                    'width': 100,
                    'height': 50,
                }
            },
            {
                selector: 'edge',
                style: {
                    'width': 2,
                    'line-color': '#ccc',
                    'target-arrow-color': '#ccc',
                    'target-arrow-shape': 'triangle'
                }
            }
        ],

        layout: {
            name: 'circle',
            fit: true,
            padding: 50,
            nodeRepulsion: 10000,
            edgeElasticity: 1000,
        }
    });

    nodes.value = cy.nodes().map(node => node.data());
    cy.on('tap', 'node', handleNodeClick);
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
    alert("Node clicked: " + node.name);
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