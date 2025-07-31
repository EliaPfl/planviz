<script setup>
import DTG from '@/components/DTG.vue';
import { useRoute } from 'vue-router';


const route = useRoute();
const id = route.params.ID;


</script>




<template>
    <DTG :ID="id" />
</template>