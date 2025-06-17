<script setup>
import axios from 'axios';
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();
const problemFile = ref(null);
const domainFile = ref(null);

function handleFileInput(event, type) {
    const file = event.target.files[0];
    if (file && file.name.endsWith('.pddl')) {
        if (type === 'problem') {
            problemFile.value = file;
            const problemLabel = document.getElementById('uploadedProblem');
            problemLabel.textContent = file.name || 'Problem PDDL file selected';
        } else if (type === 'domain') {
            domainFile.value = file;
            const domainLabel = document.getElementById('uploadedDomain');
            domainLabel.textContent = file.name || 'Domain PDDL file selected';
        }
    } else {
        alert('Please select a valid PDDL file');
    }
}

function handleSubmit() {
    if (!problemFile.value || !domainFile.value) {
        alert('Please select both problem and domain files before submitting');
        return;
    }

    const formData = new FormData();
    formData.append('problem', problemFile.value);
    formData.append('domain', domainFile.value);

    // axios.post('/api/upload', formData, {
    //     headers: {
    //         'Content-Type': 'multipart/form-data'
    //     }
    // })

    console.log('Form sent:', formData);
    alert('Files submitted successfully!');

    router.push('/causal');
}


</script>


<template>
    <div
        class="fixed top-16 left-0 w-full h-[calc(100vh-4rem)] p-6 bg-gray-700 rounded-none shadow-inner overflow-auto">
        <form @submit.prevent="handleSubmit" class="flex flex-col h-full">
            <div class="flex-1 grid grid-cols-1 md:grid-cols-2 gap-6">
                <div class="flex flex-col">
                    <label for="problemInput" class="text-lg font-medium text-gray-200 mb-2 text-center">Problem
                        PDDL</label>
                    <label for="problemInput"
                        class="flex-1 flex flex-col items-center justify-center border-2 border-dashed border-gray-500 rounded-lg p-4 cursor-pointer hover:border-blue-400 transition-colors">
                        <i class="pi pi-upload text-4xl text-gray-200"></i>
                        <span id="uploadedProblem" class="text-gray-300"></span>
                        <input @change="handleFileInput($event, 'problem')" id="problemInput" name="problem" type="file"
                            accept=".pddl" class="sr-only" />
                    </label>
                </div>

                <div class="flex flex-col">
                    <label for="domainInput" class="text-lg font-medium text-gray-200 mb-2  text-center">Domain
                        PDDL</label>
                    <label for="domainInput"
                        class="flex-1 flex flex-col items-center justify-center border-2 border-dashed border-gray-500 rounded-lg p-4 cursor-pointer hover:border-blue-400 transition-colors">
                        <i class="pi pi-upload text-4xl text-gray-200"></i>
                        <span id="uploadedDomain" class="text-gray-300"></span>
                        <input @change="handleFileInput($event, 'domain')" id="domainInput" name="domain" type="file"
                            accept=".pddl" class="sr-only" />
                    </label>
                </div>
            </div>

            <button type="submit"
                class="mt-6 w-full py-3 bg-blue-500 hover:bg-blue-600 text-white font-medium rounded-lg shadow-md transition-colors">
                Submit
            </button>
        </form>
    </div>

</template>


<style scoped></style>