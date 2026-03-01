<template>
  <div>
    <header class="flex justify-between items-start mb-6 md:mb-8">
      <div class="flex flex-col">
        <h1 class="text-2xl md:text-3xl font-extrabold text-transparent bg-clip-text bg-gradient-to-r from-emerald-600 to-teal-400 pb-1 dark:from-emerald-400 dark:to-teal-300">
          Smart Plant Box
        </h1>
        <p class="text-xs md:text-sm text-gray-500 font-medium dark:text-gray-400">Monitor and control your plant.</p>
      </div>

      <button 
        @click="toggleTheme" 
        class="p-2.5 rounded-full bg-white border border-gray-100 shadow-sm text-gray-400 hover:text-emerald-500 hover:border-emerald-200 transition-all active:scale-90 dark:bg-gray-800 dark:border-gray-700 dark:text-gray-400 dark:hover:text-yellow-400"
      >
        <svg v-if="!isDark" class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z"></path></svg>
        <svg v-else class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z"></path></svg>
      </button>
    </header>

    <div class="grid grid-cols-1 lg:grid-cols-3 gap-6 md:gap-8">
      
      <div class="lg:col-span-1 space-y-6">
        <div class="bg-gray-100 rounded-3xl aspect-square md:aspect-video lg:aspect-square flex items-center justify-center overflow-hidden shadow-sm relative group border border-gray-100 dark:bg-gray-800 dark:border-gray-700 transition-colors">
          <img src="https://picsum.photos/seed/plantbox/800/800" alt="Plant" class="w-full h-full object-cover transition-transform duration-700 group-hover:scale-105 opacity-90 hover:opacity-100" />
          
          <div class="absolute top-4 right-4 bg-white/90 backdrop-blur px-3 py-1 rounded-full text-[10px] font-bold text-emerald-600 shadow-sm flex items-center gap-1 dark:bg-gray-900/90 dark:text-emerald-400">
            <span class="w-1.5 h-1.5 rounded-full bg-emerald-500 animate-pulse"></span> LIVE
          </div>

          <button @click="isZoomed = true" class="absolute bottom-4 right-4 bg-white/80 backdrop-blur p-2 rounded-xl text-gray-600 shadow-sm opacity-0 group-hover:opacity-100 transition-opacity hover:bg-white active:scale-95 dark:bg-gray-800/80 dark:text-gray-200 dark:hover:bg-gray-800">
            <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0zM10 7v3m0 0v3m0-3h3m-3 0H7"></path></svg>
          </button>
        </div>

        <div class="bg-white rounded-2xl p-5 shadow-sm border border-gray-100 flex justify-between items-center hover:border-emerald-100 transition-colors dark:bg-gray-800 dark:border-gray-700 dark:hover:border-emerald-500/30">
          <div>
            <p class="text-[10px] font-bold text-gray-400 uppercase tracking-tight mb-1 dark:text-gray-500">Plant Status</p>
            <p class="text-base font-bold text-gray-800 leading-tight dark:text-gray-100">Healthy (Pepper Plant)</p>
          </div>
          <div class="bg-emerald-50 text-emerald-500 p-3 rounded-2xl dark:bg-emerald-500/10 dark:text-emerald-400">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7"></path></svg>
          </div>
        </div>
      </div>

      <div class="lg:col-span-2 space-y-6">
        
        <div class="bg-white rounded-3xl p-6 md:p-10 shadow-sm border border-gray-100 dark:bg-gray-800 dark:border-gray-700 transition-colors min-h-[220px] flex flex-col justify-center">
          <h3 class="text-xs font-bold text-gray-400 uppercase tracking-widest mb-10 dark:text-gray-500">Environment Sensors</h3>
          
          <div class="grid grid-cols-3 gap-x-2">
            <div v-for="sensor in sensors" :key="sensor.id" class="flex flex-col items-center group">
              <div class="relative w-28 md:w-32 h-14 md:h-16 flex justify-center items-end group-hover:-translate-y-1 transition-transform">
                <svg class="absolute top-0 left-0 w-full h-full overflow-visible" viewBox="0 0 100 50">
                  <path d="M 15 50 A 35 35 0 0 1 85 50" fill="none" class="stroke-gray-100 dark:stroke-gray-700 transition-colors" stroke-width="10" stroke-linecap="round" />
                  <path d="M 15 50 A 35 35 0 0 1 85 50" fill="none" :stroke="sensor.color" stroke-width="10" stroke-linecap="round" 
                        stroke-dasharray="110" :stroke-dashoffset="110 - (110 * sensor.percent / 100)" 
                        class="transition-all duration-1000 ease-out" />
                </svg>
                <div class="mb-[-4px] text-center flex items-baseline gap-[1px] z-10">
                  <span class="text-xl md:text-2xl font-bold text-gray-800 dark:text-white">{{ sensor.value }}</span>
                  <span class="text-xs md:text-sm font-bold text-gray-400 dark:text-gray-500" v-html="sensor.unit"></span>
                </div>
              </div>
              <span class="text-xs md:text-sm text-gray-500 mt-6 font-bold uppercase tracking-tighter dark:text-gray-400">{{ sensor.name }}</span>
            </div>
          </div>
        </div>

        <div class="grid grid-cols-1 sm:grid-cols-2 gap-4">
          <button class="bg-emerald-500 hover:bg-emerald-600 text-white font-bold py-5 px-8 rounded-2xl shadow-lg shadow-emerald-500/20 transition-all active:scale-95 text-lg flex items-center justify-center gap-2 dark:bg-emerald-600 dark:hover:bg-emerald-500">
            Water Now
          </button>
          
          <div class="flex items-center gap-4 border border-gray-100 p-5 rounded-2xl bg-white hover:border-emerald-200 transition-colors dark:bg-gray-800 dark:border-gray-700 dark:hover:border-emerald-500/30">
            <div class="flex flex-col flex-1">
              <span class="text-sm font-extrabold text-gray-800 dark:text-white">AI Irrigation</span>
              <span class="text-[10px] font-medium text-gray-400 dark:text-gray-400">System control based on data</span>
            </div>
            
            <div 
              @click="irrigationMode = irrigationMode === 'ai' ? 'manual' : 'ai'"
              :class="irrigationMode === 'ai' ? 'bg-emerald-500' : 'bg-gray-200 dark:bg-gray-600'"
              class="relative w-12 h-7 rounded-full flex items-center cursor-pointer transition-all duration-300"
            >
              <div 
                :class="irrigationMode === 'ai' ? 'translate-x-6' : 'translate-x-1'"
                class="w-5 h-5 bg-white rounded-full shadow-sm transform transition-transform duration-300"
              ></div>
            </div>
          </div>
        </div>

      </div>
    </div>

    <Transition name="fade">
      <div v-if="isZoomed" @click="isZoomed = false" class="fixed inset-0 z-[60] bg-black/80 backdrop-blur-sm flex items-center justify-center p-4 md:p-12 cursor-zoom-out">
        <div class="relative max-w-5xl w-full aspect-video bg-black rounded-3xl overflow-hidden shadow-2xl border border-gray-800">
          <img src="https://picsum.photos/seed/plantbox/1200/800" class="w-full h-full object-contain" />
          <button class="absolute top-6 right-6 bg-white/20 hover:bg-white/40 p-2 rounded-full text-white backdrop-blur transition-colors">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path></svg>
          </button>
        </div>
      </div>
    </Transition>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { ref as dbRef, onValue } from 'firebase/database' 
import { db } from '~/utils/firebase'

const { isDark, toggleTheme } = useTheme()
const isZoomed = ref(false)
const irrigationMode = useState('irrigationMode', () => 'ai')

const sensors = ref([
  { id: 1, name: 'Soil', value: '0', unit: '%', color: '#10b981', percent: 0 },
  { id: 2, name: 'Air Humid', value: '0', unit: '%', color: '#3b82f6', percent: 0 },
  { id: 3, name: 'Temp', value: '0', unit: '°C', color: '#f59e0b', percent: 0 },
])

onMounted(() => {
  const testNode = dbRef(db, 'test')

  onValue(testNode, (snapshot) => {
    const data = snapshot.val()
    if (data) {
      // Mapping ค่าจาก Firebase เฉพาะที่มีจริง
      // 1. hum (78) -> Air Humid
      sensors.value[1].value = data.hum
      sensors.value[1].percent = data.hum

      // 2. temp (24.5) -> Temp
      sensors.value[2].value = data.temp
      sensors.value[2].percent = (data.temp / 50) * 100 

      // 3. Soil -> สั่งให้เป็น 0 ตลอดจนกว่าจะมีค่าจริงจากเครื่อง
      sensors.value[0].value = '0'
      sensors.value[0].percent = 0
    }
  })
})
</script>

<style scoped>
.fade-enter-active, .fade-leave-active { transition: opacity 0.3s ease; }
.fade-enter-from, .fade-leave-to { opacity: 0; }
</style>