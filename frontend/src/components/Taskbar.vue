<script setup>
import { RouterLink } from 'vue-router'

const items = [
  { label: 'Causal Graph', iconClass: 'pi pi-chart-bar', to: '/causal' },
  { label: 'Landmark Graph', iconClass: 'pi pi-chart-line', to: '/landmark' },
  { label: 'Upload PDDL', iconClass: 'pi pi-upload', to: '/upload' }
]
</script>

<template>
  <div class="fixed top-0 left-0 right-0 bg-slate-800 dark:bg-neutral-950 text-white shadow-lg dark:shadow-2xl z-9999 border-b border-slate-700 dark:border-neutral-800">
    <div class="flex justify-between items-center h-16 px-4">
      <RouterLink to="/" class="text-lg font-bold hover:text-blue-400 transition-colors">
        PlanViz
      </RouterLink>
      <div class="flex justify-between space-x-8">
        <RouterLink v-for="item in items" :key="item.label" :to="item.to"
          class="flex flex-col items-center text-sm hover:text-blue-400 dark:hover:text-blue-300 transition-colors" active-class="text-blue-400 dark:text-blue-300">
          <i :class="item.iconClass + ' text-2xl mb-1'"></i>
          <span>{{ item.label }}</span>
        </RouterLink>
      </div>
    </div>
  </div>
</template>

<style scoped>
.router-link-active {
  color: #60A5FA;
}

/* Dark mode active link styling */
@media (prefers-color-scheme: dark) {
  .router-link-active {
    color: #93C5FD;
  }
}
</style>