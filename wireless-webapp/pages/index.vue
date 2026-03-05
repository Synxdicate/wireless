<template>
  <div>
    <header class="flex justify-between items-start mb-6 md:mb-8">
      <div class="flex flex-col">
        <h1 class="text-2xl md:text-3xl font-extrabold text-transparent bg-clip-text bg-gradient-to-r from-emerald-600 to-teal-400 pb-1 dark:from-emerald-400 dark:to-teal-300">
          Smart Plant Box
        </h1>
        <p class="text-xs md:text-sm text-gray-500 font-medium dark:text-gray-400">Monitor and control your plant manually.</p>
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
          
          <div v-if="loadingPhoto" class="absolute inset-0 flex items-center justify-center bg-gray-100 dark:bg-gray-800">
            <div class="w-8 h-8 border-2 border-emerald-500/30 border-t-emerald-500 rounded-full animate-spin"></div>
          </div>

          <div v-else-if="!latestPhotoUrl" class="absolute inset-0 flex flex-col items-center justify-center text-gray-400 dark:text-gray-600">
            <svg class="w-12 h-12 mb-2" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="1.5" d="M3 9a2 2 0 012-2h.93a2 2 0 001.664-.89l.812-1.22A2 2 0 0110.07 4h3.86a2 2 0 011.664.89l.812 1.22A2 2 0 0018.07 7H19a2 2 0 012 2v9a2 2 0 01-2 2H5a2 2 0 01-2-2V9z"></path><path stroke-linecap="round" stroke-linejoin="round" stroke-width="1.5" d="M15 13a3 3 0 11-6 0 3 3 0 016 0z"></path></svg>
            <span class="text-xs font-medium">No photo yet</span>
          </div>

          <img 
            v-else
            :src="latestPhotoUrl" 
            alt="Plant" 
            class="w-full h-full object-cover transition-transform duration-700 group-hover:scale-105 opacity-90 hover:opacity-100" 
          />
          
          <div class="absolute top-4 right-4 bg-white/90 backdrop-blur px-3 py-1.5 rounded-full shadow-sm flex items-center gap-1.5 dark:bg-gray-900/90 border border-gray-100 dark:border-gray-700">
            <svg class="w-3.5 h-3.5 text-gray-500 dark:text-gray-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z"></path>
            </svg>
            <span class="text-[10px] font-extrabold text-gray-700 dark:text-gray-200 tracking-wider">LATEST</span>
            <span class="text-[9px] font-medium text-gray-400 dark:text-gray-500 ml-1 border-l pl-2 border-gray-200 dark:border-gray-600">{{ lastPhotoTime }}</span>
          </div>

          <button @click="isZoomed = true" :disabled="!latestPhotoUrl" class="absolute bottom-4 right-4 bg-white/80 backdrop-blur p-2 rounded-xl text-gray-600 shadow-sm opacity-0 group-hover:opacity-100 transition-opacity hover:bg-white active:scale-95 dark:bg-gray-800/80 dark:text-gray-200 dark:hover:bg-gray-800 disabled:hidden">
            <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0zM10 7v3m0 0v3m0-3h3m-3 0H7"></path></svg>
          </button>
        </div>

        <div class="bg-white rounded-2xl p-5 shadow-sm border border-gray-100 flex justify-between items-center hover:border-emerald-100 transition-colors dark:bg-gray-800 dark:border-gray-700 dark:hover:border-emerald-500/30">
          <div class="flex-1 mr-3">
            <div class="flex items-center gap-2 mb-1">
              <p class="text-[10px] font-bold text-gray-400 uppercase tracking-tight dark:text-gray-500">Plant Status</p>
              <span :class="plantStatus.colorClass" class="text-[10px] font-extrabold uppercase bg-opacity-10 px-2 py-0.5 rounded-md" :style="{ backgroundColor: 'currentColor' }">
                <span class="text-white">{{ plantStatus.status }}</span>
              </span>
            </div>
            
            <div v-if="!isEditingName" @click="startEditingName" class="text-base font-bold text-gray-800 leading-tight dark:text-gray-100 cursor-pointer hover:underline decoration-dashed decoration-gray-400 flex items-center gap-1.5 group w-fit">
              {{ plantName }}
              <svg class="w-3.5 h-3.5 text-gray-300 opacity-0 group-hover:opacity-100 transition-opacity dark:text-gray-500" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15.232 5.232l3.536 3.536m-2.036-5.036a2.5 2.5 0 113.536 3.536L6.5 21.036H3v-3.572L16.732 3.732z"></path></svg>
            </div>
            <input 
              v-else 
              v-model="plantName" 
              ref="nameInput"
              @blur="savePlantName" 
              @keyup.enter="savePlantName" 
              class="w-full bg-gray-50 border border-emerald-300 text-gray-900 text-sm rounded-lg focus:ring-emerald-500 focus:border-emerald-500 block px-2.5 py-1 dark:bg-gray-900 dark:border-emerald-600 dark:text-white transition-all outline-none" 
            />
          </div>
          
          <div :class="[plantStatus.bgClass, plantStatus.iconClass]" class="p-3 rounded-2xl transition-colors duration-500 shrink-0">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" :d="plantStatus.iconPath"></path>
            </svg>
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
                        stroke-dasharray="110" :stroke-dashoffset="110 - (110 * (sensor.percent > 100 ? 100 : sensor.percent) / 100)" 
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
          <button @click="triggerWater" class="bg-cyan-500 hover:bg-cyan-600 text-white font-bold py-5 px-8 rounded-2xl shadow-lg shadow-cyan-500/20 transition-all active:scale-95 text-lg flex items-center justify-center gap-3 dark:bg-cyan-600 dark:hover:bg-cyan-500">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 2.25c-3.159 3.96-6 7.641-6 10.95 0 3.314 2.686 6 6 6s6-2.686 6-6c0-3.309-2.841-6.99-6-10.95z" />
            </svg>
            Water Now
          </button>

          <button @click="triggerPhoto" :disabled="takingPhoto" class="bg-emerald-500 hover:bg-emerald-600 text-white font-bold py-5 px-8 rounded-2xl shadow-lg shadow-emerald-500/20 transition-all active:scale-95 text-lg flex items-center justify-center gap-3 dark:bg-emerald-600 dark:hover:bg-emerald-500 disabled:opacity-60 disabled:cursor-not-allowed">
            <svg v-if="!takingPhoto" class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 9a2 2 0 012-2h.93a2 2 0 001.664-.89l.812-1.22A2 2 0 0110.07 4h3.86a2 2 0 011.664.89l.812 1.22A2 2 0 0018.07 7H19a2 2 0 012 2v9a2 2 0 01-2 2H5a2 2 0 01-2-2V9z"></path>
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 13a3 3 0 11-6 0 3 3 0 016 0z"></path>
            </svg>
            <span v-if="takingPhoto" class="w-6 h-6 border-2 border-white/30 border-t-white rounded-full animate-spin"></span>
            {{ takingPhoto ? 'Taking...' : 'Take Photo' }}
          </button>
        </div>

      </div>
    </div>

    <Transition name="fade">
      <div v-if="isZoomed" @click="isZoomed = false" class="fixed inset-0 z-[60] bg-black/80 backdrop-blur-sm flex items-center justify-center p-4 md:p-12 cursor-zoom-out">
        <div class="relative max-w-5xl w-full aspect-video bg-black rounded-3xl overflow-hidden shadow-2xl border border-gray-800">
          <img :src="latestPhotoUrl" class="w-full h-full object-contain" />
          <button class="absolute top-6 right-6 bg-white/20 hover:bg-white/40 p-2 rounded-full text-white backdrop-blur transition-colors">
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path></svg>
          </button>
        </div>
      </div>
    </Transition>
  </div>
</template>

<script setup>
import { ref, computed, nextTick, onMounted, onUnmounted } from 'vue'
import { ref as dbRef, onValue, set } from 'firebase/database' 
import { db } from '~/utils/firebase'

const { isDark, toggleTheme } = useTheme()
const isZoomed = ref(false)

const plantName = ref('Pepper Plant (Box #1)')
const isEditingName = ref(false)
const nameInput = ref(null)

const SUPABASE_URL = 'https://jojvabjckmruvmrhrrzd.supabase.co'
const SUPABASE_KEY = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImpvanZhYmpja21ydXZtcmhycnpkIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzI0NDM1MjQsImV4cCI6MjA4ODAxOTUyNH0.8XHYvfy3qfzgcf95gmeCTxLEcaQZcmzMI_4qMiRD87M'

const latestPhotoUrl = ref('')
const lastPhotoTime = ref('--/--/---- --:--')
const loadingPhoto = ref(true)
const takingPhoto = ref(false)
let pollInterval = null

const fetchLatestPhoto = async () => {
  try {
    const res = await fetch(
      `${SUPABASE_URL}/rest/v1/image?select=image_url,created_at&order=created_at.desc&limit=1`,
      { headers: { apikey: SUPABASE_KEY, Authorization: `Bearer ${SUPABASE_KEY}` } }
    )
    const data = await res.json()
    console.log('Latest photo from Supabase:', data)
    if (data && data.length > 0) {
      latestPhotoUrl.value = data[0].image_url
      const d = new Date(data[0].created_at)
      
      // 🟢 จัดฟอร์แมตให้เป็น วัน/เดือน/ปี เวลา (เช่น 03/05/2026 19:09)
      const day = String(d.getDate()).padStart(2, '0')
      const month = String(d.getMonth() + 1).padStart(2, '0')
      const year = d.getFullYear()
      const time = d.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit', hour12: false })
      
      lastPhotoTime.value = `${day}/${month}/${year} ${time}`
    }
  } catch (e) {
    console.error('Failed to fetch photo:', e)
  } finally {
    loadingPhoto.value = false
  }
}

const startEditingName = async () => {
  isEditingName.value = true
  await nextTick()
  nameInput.value?.focus()
}

const savePlantName = () => {
  isEditingName.value = false
  const nameNode = dbRef(db, 'test/plantName')
  set(nameNode, plantName.value).catch(err => console.error("Save name failed:", err))
}

const sensors = ref([
  { id: 1, name: 'Soil', value: '0', unit: '%', color: '#10b981', percent: 0 },
  { id: 2, name: 'Air Humid', value: '0', unit: '%', color: '#3b82f6', percent: 0 },
  { id: 3, name: 'Temp', value: '0', unit: '°C', color: '#f59e0b', percent: 0 },
])

const plantStatus = computed(() => {
  const humid = parseFloat(sensors.value[1].value) || 0
  const temp = parseFloat(sensors.value[2].value) || 0

  if (temp === 0 && humid === 0) {
    return {
      status: 'Syncing',
      colorClass: 'text-gray-500 dark:text-gray-400',
      bgClass: 'bg-gray-100 dark:bg-gray-700',
      iconClass: 'text-gray-500 dark:text-gray-400',
      iconPath: 'M4 4v5h.582m15.356 2A8.001 8.001 0 004.582 9m0 0H9m11 11v-5h-.581m0 0a8.003 8.003 0 01-15.357-2m15.357 2H15'
    }
  }
  if (temp > 35 || temp < 15 || humid < 30) {
    return {
      status: 'Needs Attention',
      colorClass: 'text-rose-500 dark:text-rose-400',
      bgClass: 'bg-rose-50 dark:bg-rose-500/10',
      iconClass: 'text-rose-500 dark:text-rose-400',
      iconPath: 'M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z'
    }
  }
  return {
    status: 'Healthy',
    colorClass: 'text-emerald-500 dark:text-emerald-400',
    bgClass: 'bg-emerald-50 dark:bg-emerald-500/10',
    iconClass: 'text-emerald-500 dark:text-emerald-400',
    iconPath: 'M5 13l4 4L19 7'
  }
})

onMounted(() => {
  fetchLatestPhoto()
  // Auto-refresh photo every 60 seconds
  pollInterval = setInterval(fetchLatestPhoto, 60000)

  const testNode = dbRef(db, 'test')
  onValue(testNode, (snapshot) => {
    const data = snapshot.val()
    if (data) {
      if (data.hum !== undefined) { sensors.value[1].value = data.hum; sensors.value[1].percent = data.hum }
      if (data.temp !== undefined) { sensors.value[2].value = data.temp; sensors.value[2].percent = (data.temp / 50) * 100 }
      if (data.soil_moisture !== undefined) { sensors.value[0].value = data.soil_moisture; sensors.value[0].percent = data.soil_moisture }
      if (data.plantName) plantName.value = data.plantName
    }
  })
})

onUnmounted(() => {
  if (pollInterval) clearInterval(pollInterval)
})

const triggerWater = () => {
  console.log("Watering triggered!")
}

const triggerPhoto = async () => {
  takingPhoto.value = true
  const before = latestPhotoUrl.value
  // Poll every 2s for up to 30s waiting for a new photo from ESP32
  for (let i = 0; i < 15; i++) {
    await new Promise(r => setTimeout(r, 2000))
    await fetchLatestPhoto()
    if (latestPhotoUrl.value !== before) break
  }
  takingPhoto.value = false
}
</script>

<style scoped>
.fade-enter-active, .fade-leave-active { transition: opacity 0.3s ease; }
.fade-enter-from, .fade-leave-to { opacity: 0; }
</style>