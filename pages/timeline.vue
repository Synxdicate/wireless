<template>
  <div class="max-w-4xl mx-auto pb-20">
    
    <header class="flex items-center mb-6 relative px-2 md:px-0">
      <NuxtLink to="/" class="absolute left-0 text-gray-500 hover:text-emerald-600 p-2 transition-colors dark:text-gray-400 dark:hover:text-emerald-400">
        <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7"></path></svg>
      </NuxtLink>
      <h1 class="text-xl font-bold text-gray-800 w-full text-center dark:text-white transition-colors">Growth Timeline</h1>
    </header>

    <div class="mx-2 md:mx-0 bg-white rounded-2xl p-4 shadow-sm border border-gray-100 mb-8 flex flex-col sm:flex-row justify-between items-center gap-4 transition-all hover:shadow-md dark:bg-gray-800 dark:border-gray-700">
      
      <div class="flex items-center gap-3 w-full sm:w-auto justify-start">
        <div class="bg-emerald-100 text-emerald-600 p-2.5 rounded-xl dark:bg-emerald-500/10 dark:text-emerald-400 shrink-0">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
        </div>
        <div class="flex flex-col">
          <span class="text-xs text-gray-500 font-medium dark:text-gray-400">Select Date</span>
          <span class="text-sm font-bold text-gray-800 dark:text-white">{{ formattedDate }}</span>
        </div>
      </div>
      
      <div class="flex items-center gap-1.5 md:gap-2 w-full sm:w-auto justify-between sm:justify-end">
        
        <button @click="changeDate(-1)" class="p-2.5 bg-gray-50 hover:bg-emerald-50 text-gray-500 hover:text-emerald-600 rounded-xl transition-colors dark:bg-gray-700 dark:text-gray-400 dark:hover:bg-gray-600 dark:hover:text-emerald-400 active:scale-95 border border-transparent hover:border-emerald-200 dark:hover:border-gray-500 shadow-sm">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7"></path></svg>
        </button>

        <div class="relative flex-1 sm:flex-none flex items-center justify-between w-full sm:w-[140px] bg-gray-50 border border-gray-200 rounded-xl px-3 py-2.5 hover:bg-gray-100 transition-colors dark:bg-gray-700 dark:border-gray-600 dark:hover:bg-gray-600 shadow-sm cursor-pointer overflow-hidden group">
          <span class="text-sm font-medium text-gray-800 dark:text-white pointer-events-none w-full text-center group-hover:text-emerald-600 dark:group-hover:text-emerald-400 transition-colors">
            {{ customDisplayDate }}
          </span>
          <svg class="w-4 h-4 text-gray-400 dark:text-gray-400 pointer-events-none shrink-0 group-hover:text-emerald-500 transition-colors" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
          <input 
            type="date" 
            v-model="selectedDate"
            class="absolute inset-0 w-full h-full opacity-0 cursor-pointer"
          >
        </div>

        <button @click="changeDate(1)" class="p-2.5 bg-gray-50 hover:bg-emerald-50 text-gray-500 hover:text-emerald-600 rounded-xl transition-colors dark:bg-gray-700 dark:text-gray-400 dark:hover:bg-gray-600 dark:hover:text-emerald-400 active:scale-95 border border-transparent hover:border-emerald-200 dark:hover:border-gray-500 shadow-sm">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5l7 7-7 7"></path></svg>
        </button>

      </div>
    </div>

    <div v-if="loading" class="flex flex-col items-center justify-center py-16">
      <div class="w-10 h-10 border-2 border-emerald-500/30 border-t-emerald-500 rounded-full animate-spin mb-4"></div>
      <p class="text-sm text-gray-400 dark:text-gray-500">Loading photos...</p>
    </div>

    <div v-else-if="currentTimeline.length > 0" class="px-2 md:px-0 grid grid-cols-2 md:grid-cols-3 gap-4 md:gap-6 pb-10">
      
      <div v-for="(item) in currentTimeline" :key="item.id" class="bg-white rounded-2xl shadow-sm border border-gray-100 overflow-hidden hover:shadow-md transition-all group dark:bg-gray-800 dark:border-gray-700 flex flex-col">
        
        <div class="relative aspect-square overflow-hidden bg-gray-100 dark:bg-gray-900 cursor-pointer" @click="openZoom(item.image_url)">
          <img :src="item.image_url" :alt="item.timeDisplay" class="w-full h-full object-cover transition-transform duration-700 group-hover:scale-105 opacity-90 group-hover:opacity-100">
          
          <div class="absolute top-2 left-2 bg-black/60 backdrop-blur-md text-white px-2 py-1 rounded-lg flex items-center gap-1.5 shadow-sm">
            <svg class="w-3 h-3 text-emerald-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z"></path></svg>
            <span class="text-[10px] md:text-xs font-bold tracking-wider">{{ item.timeDisplay }}</span>
          </div>

          <button class="absolute bottom-2 right-2 bg-white/80 backdrop-blur p-2 rounded-xl text-gray-600 shadow-sm opacity-0 group-hover:opacity-100 transition-opacity hover:bg-white active:scale-95 dark:bg-gray-800/90 dark:text-gray-200 dark:hover:bg-gray-700">
            <svg class="w-4 h-4 md:w-5 md:h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0zM10 7v3m0 0v3m0-3h3m-3 0H7"></path></svg>
          </button>
        </div>

        <div class="p-2 md:p-3 grid grid-cols-3 divide-x divide-gray-100 dark:divide-gray-700 bg-gray-50/50 dark:bg-gray-800/50">
          <div class="flex flex-col items-center justify-center text-center">
            <span class="text-[8px] md:text-[9px] font-bold text-gray-400 uppercase tracking-widest dark:text-gray-500 mb-0.5">Soil</span>
            <span class="text-[11px] md:text-xs font-extrabold text-emerald-500 dark:text-emerald-400">{{ item.soil != null ? item.soil + '%' : '—' }}</span>
          </div>
          <div class="flex flex-col items-center justify-center text-center">
            <span class="text-[8px] md:text-[9px] font-bold text-gray-400 uppercase tracking-widest dark:text-gray-500 mb-0.5">Humid</span>
            <span class="text-[11px] md:text-xs font-extrabold text-blue-500 dark:text-blue-400">{{ item.humid != null ? item.humid + '%' : '—' }}</span>
          </div>
          <div class="flex flex-col items-center justify-center text-center">
            <span class="text-[8px] md:text-[9px] font-bold text-gray-400 uppercase tracking-widest dark:text-gray-500 mb-0.5">Temp</span>
            <span class="text-[11px] md:text-xs font-extrabold text-amber-500 dark:text-amber-400">{{ item.temp != null ? item.temp + '°C' : '—' }}</span>
          </div>
        </div>

      </div>

    </div>

    <div v-else class="flex flex-col items-center justify-center py-16 px-4 text-center animate-fade-in">
      <div class="w-20 h-20 bg-gray-50 rounded-full flex items-center justify-center mb-4 border border-gray-100 shadow-inner dark:bg-gray-800 dark:border-gray-700">
        <svg class="w-10 h-10 text-gray-300 dark:text-gray-600" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 16l4.586-4.586a2 2 0 012.828 0L16 16m-2-2l1.586-1.586a2 2 0 012.828 0L20 14m-6-6h.01M6 20h12a2 2 0 002-2V6a2 2 0 00-2-2H6a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
      </div>
      <h3 class="text-gray-800 font-bold mb-1 dark:text-white">No Captures</h3>
      <p class="text-sm text-gray-500 dark:text-gray-400">There are no photos recorded for {{ customDisplayDate }}.</p>
    </div>

    <Teleport to="body">
      <Transition name="fade">
        <div v-if="zoomedImage" @click.self="closeZoom" class="fixed inset-0 z-[100] bg-black/80 backdrop-blur-sm flex items-center justify-center p-4 md:p-12 cursor-zoom-out">
          <div class="relative max-w-5xl w-full aspect-video bg-black rounded-3xl overflow-hidden shadow-2xl border border-gray-800 cursor-auto">
            <img :src="zoomedImage" class="w-full h-full object-contain" />
            <button @click="closeZoom" class="absolute top-6 right-6 bg-white/20 hover:bg-white/40 p-2 rounded-full text-white backdrop-blur transition-colors">
              <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path></svg>
            </button>
          </div>
        </div>
      </Transition>
    </Teleport>

  </div>
</template>

<script setup>
import { ref, computed, watch, onMounted } from 'vue'

useHead({ title: 'Growth Timeline | Smart Plant Box' })

const SUPABASE_URL = 'https://jojvabjckmruvmrhrrzd.supabase.co'
const SUPABASE_KEY = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImpvanZhYmpja21ydXZtcmhycnpkIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzI0NDM1MjQsImV4cCI6MjA4ODAxOTUyNH0.8XHYvfy3qfzgcf95gmeCTxLEcaQZcmzMI_4qMiRD87M'

const today = new Date().toISOString().split('T')[0]
const selectedDate = ref(today)
const loading = ref(false)
const currentTimeline = ref([])

const zoomedImage = ref(null)
const openZoom = (url) => { zoomedImage.value = url }
const closeZoom = () => { zoomedImage.value = null }

const formattedDate = computed(() => {
  const dateObj = new Date(selectedDate.value)
  return dateObj.toLocaleDateString('en-US', { month: 'short', day: 'numeric', year: 'numeric' })
})

const customDisplayDate = computed(() => {
  if (!selectedDate.value) return ''
  const [year, month, day] = selectedDate.value.split('-')
  return `${day}/${month}/${year}`
})

const changeDate = (daysToAdd) => {
  const currentDate = new Date(selectedDate.value)
  currentDate.setDate(currentDate.getDate() + daysToAdd)
  
  const year = currentDate.getFullYear()
  const month = String(currentDate.getMonth() + 1).padStart(2, '0')
  const day = String(currentDate.getDate()).padStart(2, '0')
  
  selectedDate.value = `${year}-${month}-${day}`
}

const fetchPhotos = async (date) => {
  loading.value = true
  currentTimeline.value = []
  try {
    // 🟢 เปลี่ยนกลับมาเป็น select=* แล้วครับ รับรองว่าข้อมูลมาครบ ไม่ติดบั๊กแน่นอน
    const res = await fetch(
      `${SUPABASE_URL}/rest/v1/image?select=*&order=created_at.asc`,
      { headers: { apikey: SUPABASE_KEY, Authorization: `Bearer ${SUPABASE_KEY}` } }
    )
    const data = await res.json()

    if (data.error) {
      console.error("Supabase Error:", data.message)
      return
    }

    currentTimeline.value = (data || [])
      .filter(row => {
        const rowDate = new Date(row.created_at).toLocaleDateString('en-CA')
        return rowDate === date
      })
      .map(row => {
        const d = new Date(row.created_at)
        return {
          id: row.id,
          image_url: row.image_url,
          timeDisplay: d.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit', hour12: true }),
          soil: row.soil ?? null,
          humid: row.humid ?? null,
          temp: row.temp ?? null
        }
      })
  } catch (e) {
    console.error('Failed to fetch timeline:', e)
  } finally {
    loading.value = false
  }
}

onMounted(() => fetchPhotos(selectedDate.value))
watch(selectedDate, (date) => fetchPhotos(date))
</script>

<style scoped>
.animate-fade-in {
  animation: fadeIn 0.4s ease-out;
}
@keyframes fadeIn {
  from { opacity: 0; transform: translateY(10px); }
  to { opacity: 1; transform: translateY(0); }
}
.fade-enter-active, .fade-leave-active { 
  transition: opacity 0.3s ease; 
}
.fade-enter-from, .fade-leave-to { 
  opacity: 0; 
}
</style>