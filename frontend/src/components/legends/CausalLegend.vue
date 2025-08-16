<template>
  <BaseLegend title="Legende" subtitle="Causal Graph Elemente"
    description="Der Causal Graph zeigt Abhängigkeiten zwischen Zustandsvariablen einer Planungsaufgabe.">
    <template #legend-content>
      <div class="space-y-3">
        <!-- Node Types -->
        <div class="mb-4">
          <h4 class="font-medium text-slate-700 dark:text-neutral-100 mb-2">Knoten</h4>

          <div class="flex items-center gap-3 mb-2">
            <div class="flex-shrink-0">
              <div class="w-10 h-6 bg-slate-400 dark:bg-neutral-500 rounded flex items-center justify-center"></div>
            </div>
            <div>
              <span class="font-medium text-slate-800 dark:text-neutral-200">Variable</span>
              <p class="text-sm text-slate-600 dark:text-neutral-400">ohne Zielwert</p>
            </div>
          </div>

          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <svg viewBox="0 0 100 70" class="w-10 h-6 text-slate-400 dark:text-neutral-500 fill-current">
                <polygon points="30,0 70,0 100,20 100,50 70,70 30,70 0,50 0,20" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-slate-800 dark:text-neutral-200">Variable</span>
              <p class="text-sm text-slate-600 dark:text-neutral-400">mit Zielwert</p>
            </div>
          </div>
        </div>

        <!-- Edge Types -->
        <div>
          <h4 class="font-medium text-slate-700 dark:text-neutral-100 mb-2">Kanten</h4>

          <div class="flex items-center gap-3">
            <div class="flex-shrink-0">
              <svg width="40" height="20" viewBox="0 0 40 20">
                <line x1="5" y1="10" x2="35" y2="10" stroke="#666" stroke-width="2" />
                <polygon points="35,10 30,7 30,13" fill="#666" />
              </svg>
            </div>
            <div>
              <span class="font-medium text-slate-800 dark:text-neutral-200">Abhängigkeit</span>
              <p class="text-sm text-slate-600 dark:text-neutral-400">Kausale Beziehung</p>
            </div>
          </div>
        </div>

        <!-- Colors -->
        <div class="mb-4">
          <h4 class="font-medium text-slate-700 dark:text-neutral-100 mb-2">Farben</h4>
          <p class="text-sm text-slate-600 dark:text-neutral-400">Jede Farbe markiert eine starke Zusammenhangskomponente im Graphen.
          </p>
        </div>

        <!-- Interactions -->
         <div class="mb-4">
          <h4 class="font-medium text-slate-700 dark:text-neutral-100 mb-2">Interaktionen</h4>
          <p class="text-sm text-slate-600 dark:text-neutral-400">
            Doppelklick auf einen Knoten öffnet den zugehörigen Domain Transition Graph.
          </p>
        </div>

      </div>
    </template>
  </BaseLegend>
</template>

<script setup>
import BaseLegend from './BaseLegend.vue'
</script>
