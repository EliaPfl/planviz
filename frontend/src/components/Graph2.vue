<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import {elements} from "@/assets/dtg_8.json"

const router = useRouter();
const nodes = ref([]);
const edges = ref([]);

onMounted(() => {

    const cy = cytoscape({
        container: document.getElementById('cy'),

        elements: elements,

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
                'width': 'label',
                'height': 'label',
              }
            },
            {
              selector: 'edge',
              style: {
                'label': 'data(label)',
                'font-size': 10,
                'text-rotation': 'autorotate',
                'text-margin-y': -10,
                'text-margin-x': 10,
                'text-wrap': 'wrap',
                'width': 2,
                'line-color': '#ccc',
                'target-arrow-color': '#ccc',
                'target-arrow-shape': 'triangle',
                'target-arrow-scale': 3,
                'curve-style': 'bezier'
              }
            },
            {
              selector: 'node.init',
              style: {
                'border-color': '#388E3C',
                'border-width': '5px',
              }
            },
            {
              selector: 'node.goal',
              style: {
                'border-color': '#D32F2F',
                'border-width': '5px',
              }
            },
            {
              selector: 'node.init.goal',
              style: {
                'border-color': '#FBC02D',
                'border-width': '5px',

              }
            },
            
          ],

          layout: {
            name: 'cose',
            fit: true,
            padding: 50,
            nodeRepulsion: 100000,
            edgeElasticity: 100000,
            animate: false,
            randomize: false,
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