<script setup>
import { ref, onMounted } from 'vue';
import cytoscape from 'cytoscape';
import { useRouter } from 'vue-router';
import axios, { AxiosError } from 'axios';
import Swal from 'sweetalert2';
import DTGLegend from './legends/DTGLegend.vue';

const router = useRouter();
const nodes = ref([]);
const edges = ref([]);
const elements = ref([]);
const selectedElement = ref(null);
const selectedType = ref('node'); // 'node' or 'edge'
const props = defineProps({
  ID: {
    type: String,
    required: true
  }
});

onMounted(() => {
  axios.get(`/api/dtg/${props.ID}`)
    .then(response => {
      let colors = generateColors(response.data["metadata"]["num_sccs"]);

      Object.values(response.data["elements"]["nodes"]).forEach(el => {
        if (el.data && el.data.scc_id !== undefined) {
          el.data.color = colors[el.data.scc_id];
          el.data.fontColor = getContrastColor(colors[el.data.scc_id]);
        }
      });

      Object.values(response.data["elements"]["edges"]).forEach(el => {
        if (el.data && el.data.label) {
          // Speichere das ursprüngliche Label für detaillierte Anzeige
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
      elements.value = response.data.elements;
      const cy = cytoscape({
        container: document.getElementById('cy'),

        elements: elements.value,

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
              'border-width': '4px',
            }
          },
          {
            selector: 'node.goal',
            style: {
              'border-color': '#D32F2F',
              'border-width': '4px',
            }
          },
          {
            selector: 'node.init.goal',
            style: {
              'border-color': '#FBC02D',
              'border-width': '4px',

            }
          },

        ],

        layout: {
          name: 'breadthfirst',
          fit: true,
          padding: 50,
          animate: false,
        }
      });
      nodes.value = cy.nodes().map(node => node.data());
      edges.value = cy.edges().map(edge => edge.data());
      cy.on('tap', 'node', handleNodeClick);
      cy.on('tap', 'edge', handleEdgeClick);
    })
    .catch(error => {
      if (error instanceof AxiosError) {
        if (error.response && error.response.status === 405) {
          Swal.fire({
            icon: 'error',
            title: 'No Domain Transition Graph Available',
            text: 'Please upload PDDL files first to generate a Domain Transition Graph.',
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

function handleEdgeClick(event) {
  const edge = event.target;
  const edgeData = edge.data();
  selectedElement.value = edgeData;
  selectedType.value = 'edge';
  
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
    const hue = 180 - ((i * 360 / count) % 360);    // Gleichmäßig verteilte Farbtöne
    const saturation = 70 + (i % 3) * 10;   // Variiert zwischen 70-90%
    const lightness = 50 + (i % 2) * 10;    // Variiert zwischen 50-60%
    colors.push(`hsl(${hue}, ${saturation}%, ${lightness}%)`);
  }
  return colors;
}

function getContrastColor(hslColor) {
  return `rgb(0, 0, 0)`;
}
</script>

<template>
  <div class="flex h-[calc(100vh-4rem)] mt-16">
    <div id="left" class="w-2/3 p-4 relative">
      <RouterLink to="/causal" class="absolute top-5 left-5 z-10 text-2xl mb-1 bg-white rounded w-12 h-12 flex items-center justify-center shadow">
        <i class="pi pi-arrow-left"></i>
      </RouterLink>
      <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
      <DTGLegend />
    </div>

    <div id="right" class="w-1/3 p-4">
      <div class="bg-gray-50 h-full overflow-y-auto rounded-lg shadow-inner p-4">
        <div class="mb-4">
          <div class="flex space-x-2 mb-4">
            <button 
              @click="selectedType = 'node'; selectedElement = null" 
              :class="['px-3 py-1 rounded text-sm', selectedType === 'node' ? 'bg-blue-500 text-white' : 'bg-gray-200 text-gray-700']">
              Nodes
            </button>
            <button 
              @click="selectedType = 'edge'; selectedElement = null" 
              :class="['px-3 py-1 rounded text-sm', selectedType === 'edge' ? 'bg-blue-500 text-white' : 'bg-gray-200 text-gray-700']">
              Edges
            </button>
          </div>
        </div>

        <!-- Selected Element Details -->
        <div v-if="selectedElement && selectedType === 'node'" class="mb-6 p-4 bg-blue-50 rounded-lg border-l-4 border-blue-500">
          <h3 class="text-lg font-semibold mb-2 text-blue-800">Selected Node</h3>
          <div class="space-y-2">
            <p><strong>ID:</strong> {{ selectedElement.id }}</p>
            <p><strong>Name:</strong> {{ selectedElement.name }}</p>
            <p v-if="selectedElement.scc_id !== undefined"><strong>SCC ID:</strong> {{ selectedElement.scc_id }}</p>
          </div>
        </div>

        <div v-if="selectedElement && selectedType === 'edge'" class="mb-6 p-4 bg-green-50 rounded-lg border-l-4 border-green-500">
          <h3 class="text-lg font-semibold mb-2 text-green-800">Selected Edge</h3>
          <div class="space-y-3">
            <p><strong>ID:</strong> {{ selectedElement.id }}</p>
            <p><strong>From:</strong> {{ getNodeName(selectedElement.source) }}</p>
            <p><strong>To:</strong> {{ getNodeName(selectedElement.target) }}</p>
            <div v-if="selectedElement.originalLabel">
              <strong>Actions:</strong>
              <div class="mt-2 space-y-2">
                <div v-for="(params, action) in selectedElement.originalLabel" :key="action" 
                     class="bg-white rounded border p-3">
                  <div class="font-semibold text-green-700 mb-2 capitalize">{{ action }}</div>
                  <div class="space-y-1">
                    <div v-for="(param, index) in params" :key="index" 
                         class="text-sm bg-gray-100 rounded px-2 py-1 font-mono">
                      ({{ param }})
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Nodes List -->
        <div v-show="selectedType === 'node'">
          <h2 class="text-lg font-semibold mb-4">All Nodes</h2>
          <ul class="space-y-2">
            <li v-for="node in nodes" :key="node.id" :id="`node-` + node.id"
              @click="selectedElement = node; selectedType = 'node'"
              :class="['p-2 rounded hover:bg-blue-50 cursor-pointer', selectedElement?.id === node.id && selectedType === 'node' ? 'bg-blue-100 border border-blue-300' : '']">
              <strong>{{ node.name }}</strong>
              <p v-if="node.beschreibung" class="text-sm text-gray-600">
                {{ node.beschreibung }}
              </p>
            </li>
          </ul>
        </div>

        <!-- Edges List -->
        <div v-show="selectedType === 'edge'">
          <h2 class="text-lg font-semibold mb-4">All Edges</h2>
          <ul class="space-y-2">
            <li v-for="edge in edges" :key="edge.id" :id="`edge-` + edge.id"
              @click="selectedElement = edge; selectedType = 'edge'"
              :class="['p-2 rounded hover:bg-green-50 cursor-pointer', selectedElement?.id === edge.id && selectedType === 'edge' ? 'bg-green-100 border border-green-300' : '']">
              <div class="text-sm">
                <strong>{{ getNodeName(edge.source) }} → {{ getNodeName(edge.target) }}</strong>
                <p v-if="edge.label" class="text-gray-600 mt-1">
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


<style scoped>
</style>