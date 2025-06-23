<script setup>
import axios from 'axios';
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();

const fileInput = ref([]);

function handleFileInput(event) {
    if (!event.target.files || event.target.files.length === 0) {
        alert('No file selected');
        return;
    }
    for (let i = 0; i < event.target.files.length; i++) {
        const file = event.target.files[i];
        if (!file.name.endsWith('.pddl')) {
            alert('Please select a valid PDDL file');
            return;
        }
        fileInput.value.push(file);
    }
}

function handleSubmit() {
    if (!fileInput.value || fileInput.value.length === 0) {
        alert('Please select at least one file to upload');
        return;
    } else if (fileInput.value.length > 2) {
        alert('You can only upload a maximum of 2 files');
        return;
    }

    const formData = new FormData();
    fileInput.value.forEach(file => {
        formData.append('files', file);
    });

    // axios.post('/api/upload', formData, {
    //     headers: {
    //         'Content-Type': 'multipart/form-data'
    //     }
    // })

    console.log('Form sent:', formData);
    alert('Files submitted successfully!');

    router.push('/causal');
}

function removeFile(index) {
    fileInput.value.splice(index, 1);
}

</script>


<template>
    <div
        class="fixed top-16 left-0 w-full h-[calc(100vh-4rem)] p-6 bg-gray-700 shadow-inner overflow-auto flex flex-col items-center">
        <div class="w-full max-w-2xl mx-auto p-6 bg-gray-800 text-gray-50 rounded-lg shadow-md mb-8">
            <h2 class="text-lg font-semibold mb-4">Uploaded Files</h2>
            <ul class="divide-y divide-gray-700">
                <li v-for="(file, index) in fileInput" :key="index" class="flex justify-between items-center py-2">
                    <span>{{ file.name }}</span>
                    <i @click="removeFile(index)" class="pi pi-times cursor-pointer hover:text-red-400"></i>
                </li>
            </ul>
        </div>

        <form @submit.prevent="handleSubmit" class="w-full max-w-2xl mx-auto flex flex-col space-y-6">
            <label for="domainInput" class="w-full h-64 flex flex-col items-center justify-center p-6
               border-2 border-dashed border-gray-600 rounded-lg
               hover:border-blue-400 transition-colors">
                <i class="pi pi-upload text-5xl text-gray-400 mb-4"></i>
                <input id="domainInput" name="domain" type="file" accept=".pddl" multiple class="sr-only"
                    @change="handleFileInput($event, 'domain')" />
            </label>

            <button type="submit"
                class="w-full px-8 py-3 bg-blue-500 hover:bg-blue-600 text-white font-medium rounded-lg shadow transition-colors">
                Submit
            </button>
        </form>
    </div>
</template>


<style scoped></style>