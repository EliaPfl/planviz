<template>
  <div class="legend-container absolute top-5 right-5 z-20">
    <!-- Toggle Button -->
    <button @click="toggleLegend"
      class="bg-blue-600 text-white p-3 rounded-lg shadow-lg hover:bg-blue-700 transition-colors duration-200 flex items-center gap-2 relative"
      :class="{ 'rounded-b-none': isOpen }">
      <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
          d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
      </svg>
      {{ title }}
      <svg class="w-4 h-4 transition-transform duration-200" :class="{ 'rotate-180': isOpen }" fill="none"
        stroke="currentColor" viewBox="0 0 24 24">
        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 9l-7 7-7-7" />
      </svg>
    </button>

    <!-- Legend Content -->
    <div v-show="isOpen"
      class="bg-white border border-gray-200 rounded-b-lg rounded-tl-lg shadow-lg p-4 min-w-80 transition-all duration-200 absolute right-0 top-[calc(100%+1px)] z-30"
      :class="{ 'opacity-100 translate-y-0': isOpen, 'opacity-0 -translate-y-2': !isOpen }">
      <h3 class="text-lg font-semibold mb-3 text-gray-800">{{ subtitle }}</h3>

      <!-- Slot for legend content -->
      <slot name="legend-content"></slot>

      <div v-if="description" class="mt-4 pt-3 border-t border-gray-200">
        <p class="text-xs text-gray-500">
          {{ description }}
        </p>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'

// Props for the legend component
const props = defineProps({
  title: {
    type: String,
    default: 'Legende'
  },
  subtitle: {
    type: String,
    default: 'Erklärung'
  },
  description: {
    type: String,
    default: null
  }
})

const isOpen = ref(false)

const toggleLegend = () => {
  isOpen.value = !isOpen.value
}
</script>

<style scoped>
.legend-container {
  user-select: none;
}

/* Smooth transition for the legend content */
.transition-all {
  transition: all 0.2s ease-in-out;
}
</style>
