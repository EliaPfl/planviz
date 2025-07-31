<script setup>
import { RouterLink } from 'vue-router'

const items = [
  { label: 'Causal Graph', iconClass: 'pi pi-chart-bar', to: '/causal' },
  { label: 'Upload PDDL', iconClass: 'pi pi-upload', to: '/upload' }
]
</script>

<template>
  <div class="fixed top-0 left-0 right-0 bg-gray-800 text-white shadow-lg z-9999">
    <div class="flex justify-between items-center h-16 px-4">
      <RouterLink to="/" class="text-lg font-bold hover:text-blue-400">
        PlanViz
      </RouterLink>
      <div class="flex justify-between space-x-8">
        <RouterLink v-for="item in items" :key="item.label" :to="item.to"
          class="flex flex-col items-center text-sm hover:text-blue-400" active-class="text-blue-400">
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
</style>