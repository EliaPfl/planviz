<template>
  <BaseLegend title="Legende" subtitle="Landmark Graph Elemente" description="Der Landmark-Graph zeigt notwendige Zwischenziele (Landmarks) im Planungsprozess sowie Abhängigkeitsrelationen zwischen ihnen. 
    Knoten repräsentieren Landmarks, Kanten geben vor, welche Landmark vor einer anderen erreicht werden muss.">
    <template #legend-content>
      <div class="space-y-3">
        <!-- Node Types -->
        <div class="mb-4">
          <h4 class="font-medium text-gray-700 mb-2">Knoten</h4>

          <div class="flex items-center gap-3 mb-2">
            <div class="flex-shrink-0">
              <svg class="w-10 h-6 text-green-700 fill-current">
                <ellipse cx="20" cy="12" rx="20" ry="12"/>
              </svg>
            </div>
            <div>
              <span class="font-medium text-gray-800">Landmarks</span>
              <p class="text-sm text-gray-600">aus fastdownward</p>
            </div>
          </div>
        </div>

        <div>
          <h4 class="font-medium text-gray-700 mb-2">Kanten</h4>
          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <svg width="40" height="20" viewBox="0 0 40 20">
                <line x1="5" y1="10" x2="30" y2="10" stroke="#666" stroke-width="2" />
                <polygon points="35,10 30,7 30,13" fill="#666" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-gray-800">NECESSARY & GREEDY_NECESSARY</span>
            </div>
          </div>

          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <svg width="40" height="20" viewBox="0 0 40 20">
                <line x1="5" y1="10" x2="30" y2="10" stroke="#666" stroke-width="2" stroke-dasharray="5,3" />
                <polygon points="35,10 30,7 30,13" fill="#666" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-gray-800">NATURAL</span>
            </div>
          </div>

          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <svg width="40" height="20" viewBox="0 0 40 20">
                <line x1="5" y1="10" x2="30" y2="10" stroke="#666" stroke-width="2" stroke-dasharray="2,2"/>
                <polygon points="35,10 30,7 30,13" fill="#666" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-gray-800">REASONABLE</span>
            </div>
          </div>
        </div>
      </div>
    </template>
  </BaseLegend>
</template>

<script setup>
import BaseLegend from './BaseLegend.vue'
</script>
