<template>
  <div class="max-w-2xl mx-auto pb-20 relative min-h-screen">
    
    <header class="flex items-center mb-6 relative">
      <NuxtLink to="/" class="absolute left-0 text-gray-500 hover:text-gray-800 dark:text-gray-400 dark:hover:text-white p-2 transition-colors">
        <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7"></path></svg>
      </NuxtLink>
      <h1 class="text-xl font-bold text-gray-800 dark:text-white w-full text-center">Settings & Automation</h1>
    </header>

    <div class="space-y-8">
      
      <section class="px-2">
        <h2 class="text-base font-bold text-gray-800 dark:text-gray-200 mb-4">Irrigation Mode</h2>
        <div class="grid grid-cols-2 gap-4">
          <button 
            @click="irrigationMode = 'ai'"
            :class="irrigationMode === 'ai' 
              ? 'bg-emerald-600 text-white shadow-md' 
              : 'bg-white text-gray-600 border border-gray-200 dark:bg-gray-800 dark:text-gray-300 dark:border-gray-700'"
            class="rounded-xl p-4 flex flex-col items-center justify-center text-center transition-all h-28"
          >
            <span class="font-bold text-sm mb-1">AI Automated</span>
            <span :class="irrigationMode === 'ai' ? 'text-emerald-100' : 'text-gray-400 dark:text-gray-500'" class="text-[10px] leading-tight px-2">
              Smart watering based on plant needs
            </span>
          </button>

          <button 
            @click="irrigationMode = 'manual'"
            :class="irrigationMode === 'manual' 
              ? 'bg-emerald-600 text-white shadow-md' 
              : 'bg-white text-gray-800 border border-gray-200 dark:bg-gray-800 dark:text-gray-200 dark:border-gray-700'"
            class="rounded-xl p-4 flex flex-col items-center justify-center text-center transition-all h-28"
          >
            <span class="font-bold text-sm">Manual<br>Schedule</span>
          </button>
        </div>
      </section>

      <div class="min-h-[140px]"> 
        <section v-if="irrigationMode === 'manual'" class="bg-gray-100 dark:bg-gray-800 rounded-3xl p-6 animate-fade-in transition-colors">
          <h2 class="text-sm font-semibold text-gray-500 dark:text-gray-400 mb-4">Manual Watering Schedule</h2>
          <div class="space-y-4">
            <div @click="manualSchedule = '1'" class="flex items-center justify-between cursor-pointer group">
              <div class="flex items-center gap-3">
                <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors"
                     :class="manualSchedule === '1' ? 'border-emerald-600' : 'border-gray-400 dark:border-gray-600 group-hover:border-emerald-400'">
                  <div v-if="manualSchedule === '1'" class="w-2.5 h-2.5 bg-emerald-600 rounded-full"></div>
                </div>
                <span class="text-sm text-gray-800 dark:text-gray-200 font-medium">1 time/day</span>
              </div>
              <span class="text-xs font-semibold text-gray-600 dark:text-gray-300 bg-gray-200/80 dark:bg-gray-700 px-2 py-1 rounded-md">07:00 AM</span>
            </div>

            <div @click="manualSchedule = '2'" class="flex items-center justify-between cursor-pointer group">
              <div class="flex items-center gap-3">
                <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors"
                     :class="manualSchedule === '2' ? 'border-emerald-600' : 'border-gray-400 dark:border-gray-600 group-hover:border-emerald-400'">
                  <div v-if="manualSchedule === '2'" class="w-2.5 h-2.5 bg-emerald-600 rounded-full"></div>
                </div>
                <span class="text-sm text-gray-800 dark:text-gray-200 font-medium">2 times/day</span>
              </div>
              <span class="text-xs font-semibold text-gray-600 dark:text-gray-300 bg-gray-200/80 dark:bg-gray-700 px-2 py-1 rounded-md">07:00 AM & 05:00 PM</span>
            </div>
          </div>
        </section>

        <section v-else class="bg-emerald-50 dark:bg-emerald-900/20 rounded-3xl p-6 border border-emerald-100 dark:border-emerald-800 flex gap-4 items-start animate-fade-in transition-colors">
          <div class="bg-emerald-100 dark:bg-emerald-800 text-emerald-600 dark:text-emerald-400 p-2 rounded-xl mt-1">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 10V3L4 14h7v7l9-11h-7z"></path></svg>
          </div>
          <div>
            <h3 class="text-sm font-bold text-emerald-800 dark:text-emerald-300 mb-1">AI is in control</h3>
            <p class="text-xs text-emerald-600/80 dark:text-emerald-400 leading-relaxed">
              Sit back and relax! The system will automatically water your plant based on real-time soil moisture and temperature data.
            </p>
          </div>
        </section>
      </div>

      <section class="px-2">
        <h2 class="text-base font-bold text-gray-800 dark:text-gray-200 mb-4">Photo Capture Frequency</h2>
        <div class="space-y-4">
          <label v-for="(item, index) in photoOptions" :key="index" class="flex items-center gap-3 cursor-pointer group">
            <input type="radio" :value="item.value" v-model="photoFrequency" class="hidden">
            <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors"
                 :class="photoFrequency === item.value ? 'border-emerald-600' : 'border-gray-300 dark:border-gray-600 group-hover:border-emerald-400'">
              <div v-if="photoFrequency === item.value" class="w-2.5 h-2.5 bg-emerald-600 rounded-full"></div>
            </div>
            <span class="text-sm text-gray-800 dark:text-gray-200 font-medium">{{ item.label }}</span>
          </label>
        </div>
      </section>

    </div>

    <div 
      class="fixed z-[100] flex items-center gap-2 bg-gray-800 dark:bg-gray-700 border border-transparent dark:border-gray-600 text-white px-5 py-2.5 rounded-full text-sm shadow-xl transition-all duration-300
             bottom-24 left-1/2 -translate-x-1/2 
             md:bottom-10 md:left-auto md:right-10 md:translate-x-0"
      :class="showToast ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-4 pointer-events-none'"
    >
      <span v-if="isSaving" class="w-4 h-4 border-2 border-white/30 border-t-white rounded-full animate-spin"></span>
      <svg v-else class="w-4 h-4 text-emerald-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7"></path></svg>
      {{ isSaving ? 'Saving...' : 'Settings saved' }}
    </div>

  </div>
</template>

<script setup>
import { ref, watch } from 'vue'

useHead({ title: 'Settings | Smart Plant Box' })

// State management
const irrigationMode = useState('irrigationMode', () => 'ai')  
const manualSchedule = ref('1')       
const photoFrequency = ref('morning') 

// UI State สำหรับ Auto-save
const isSaving = ref(false)
const showToast = ref(false)
let toastTimeout = null

const photoOptions = [
  { label: 'Morning', value: 'morning' },
  { label: 'Morning & Evening', value: 'morning_evening' },
  { label: 'Morning, Noon & Evening', value: 'morning_noon_evening' }
]

const autoSaveSettings = async () => {
  isSaving.value = true
  showToast.value = true
  clearTimeout(toastTimeout) 

  try {
    console.log('Sending to Firebase:', {
      mode: irrigationMode.value,
      schedule: manualSchedule.value,
      photo: photoFrequency.value
    })

    await new Promise(resolve => setTimeout(resolve, 800))

  } catch (error) {
    console.error('Save failed:', error)
  } finally {
    isSaving.value = false
    toastTimeout = setTimeout(() => {
      showToast.value = false
    }, 2000)
  }
}

watch(
  [irrigationMode, manualSchedule, photoFrequency], 
  () => {
    autoSaveSettings()
  },
  { deep: true }
)
</script>

<style scoped>
.animate-fade-in {
  animation: fadeIn 0.3s ease-out;
}
@keyframes fadeIn {
  from { opacity: 0; transform: translateY(-10px); }
  to { opacity: 1; transform: translateY(0); }
}
</style>