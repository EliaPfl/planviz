<script setup>
import Graph2 from '@/components/Graph2.vue';
import { useRoute } from 'vue-router';


const route = useRoute();
const id = route.params.ID;


</script>




<template><Graph2 :ID="id"/></template>