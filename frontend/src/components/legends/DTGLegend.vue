<template>
  <BaseLegend title="Legende" subtitle="DTG Elemente"
    description="Der Domain Transition Graph zeigt mögliche Übergänge zwischen Zuständen einer Variable.">
    <template #legend-content>
      <div class="space-y-3">
        <!-- Node Types -->
        <div class="mb-4">
          <h4 class="font-medium text-gray-700 mb-2">Knoten</h4>

          <div class="flex items-center gap-3 mb-2">
            <div class="flex-shrink-0">
              <div class="w-10 h-6 bg-slate-400 rounded border-4 border-green-600"></div>
            </div>
            <div>
              <span class="font-medium text-gray-800">Startwert</span>
              <p class="text-sm text-gray-600">Anfangswert der Variable</p>
            </div>
          </div>

          <div class="flex items-center gap-3 mb-2">
            <div class="flex-shrink-0">
              <div class="w-10 h-6 bg-slate-400 rounded border-4 border-red-600"></div>
            </div>
            <div>
              <span class="font-medium text-gray-800">Zielwert</span>
              <p class="text-sm text-gray-600">Gewünschter Endwert</p>
            </div>
          </div>

          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <div class="w-10 h-6 bg-slate-400 rounded"></div>
            </div>
            <div>
              <span class="font-medium text-gray-800">Zwischenwert</span>
              <p class="text-sm text-gray-600"></p>
            </div>
          </div>
        </div>

        <!-- Edge Types -->
        <div>
          <h4 class="font-medium text-gray-700 mb-2">Kanten</h4>

          <div class="flex items-center gap-3 mb-2">
            <div class="flex-shrink-0">
              <svg width="40" height="20" viewBox="0 0 40 20">
                <line x1="5" y1="10" x2="35" y2="10" stroke="#666" stroke-width="2" />
                <polygon points="35,10 30,7 30,13" fill="#666" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-gray-800">Übergang</span>
              <p class="text-sm text-gray-600">mit Aktionen</p>
            </div>
          </div>
        </div>

        <!-- Colors -->
        <div>
          <h4 class="font-medium text-gray-700 mb-2">Farben</h4>
          <p class="text-sm text-gray-600">Jede Farbe markiert eine starke Zusammenhangskomponente im Graphen.
          </p>
        </div>
      </div>
    </template>
  </BaseLegend>
</template>

<script setup>
import BaseLegend from './BaseLegend.vue'
</script>
