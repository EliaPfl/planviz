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
      cy.on('tap', 'node', handleNodeClick);
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
  const nodeElement = document.getElementById(`node-${nodeData.id}`);

  if (nodeElement) {
    nodeElement.scrollIntoView({ behavior: 'smooth', block: 'center' });
    nodeElement.classList.add('node-highlight');
    nodeElement.addEventListener('animationend', () => {
      nodeElement.classList.remove('node-highlight');
    }, { once: true });
  }
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
      <RouterLink to="/causal" class="absolute top-5 left-5 z-10 text-2xl mb-1"><i class="pi pi-arrow-left"></i>
      </RouterLink>
      <div id="cy" class="w-full h-full bg-gray-100 rounded-lg shadow-inner"></div>
      <DTGLegend />
    </div>

    <div id="right" class="w-1/3 p-4">
      <div class="bg-gray-50 h-full overflow-y-auto rounded-lg shadow-inner p-4">
        <h2 class="text-lg font-semibold mb-4">Node Details</h2>
        <ul class="space-y-2">
          <li v-for="node in nodes" :key="node.id" :id="`node-` + node.id"
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