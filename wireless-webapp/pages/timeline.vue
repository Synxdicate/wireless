<template>
  <div class="max-w-2xl mx-auto pb-20">
    
    <header class="flex items-center mb-6 relative">
      <NuxtLink to="/" class="absolute left-0 text-gray-500 hover:text-emerald-600 p-2 transition-colors dark:text-gray-400 dark:hover:text-emerald-400">
        <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7"></path></svg>
      </NuxtLink>
      <h1 class="text-xl font-bold text-gray-800 w-full text-center dark:text-white transition-colors">Growth Timeline</h1>
    </header>

    <div class="bg-white rounded-2xl p-4 shadow-sm border border-gray-100 mb-8 flex justify-between items-center transition-all hover:shadow-md dark:bg-gray-800 dark:border-gray-700">
      <div class="flex items-center gap-3">
        <div class="bg-emerald-100 text-emerald-600 p-2.5 rounded-xl dark:bg-emerald-500/10 dark:text-emerald-400">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
        </div>
        <div class="flex flex-col">
          <span class="text-xs text-gray-500 font-medium dark:text-gray-400">Select Date</span>
          <span class="text-sm font-bold text-gray-800 dark:text-white">{{ formattedDate }}</span>
        </div>
      </div>
      
      <div class="relative">
        <input 
          type="date" 
          v-model="selectedDate"
          class="bg-gray-50 border border-gray-200 text-gray-800 text-sm font-medium rounded-xl focus:ring-emerald-500 focus:border-emerald-500 block px-3 py-2.5 outline-none cursor-pointer hover:bg-gray-100 transition-colors dark:bg-gray-700 dark:border-gray-600 dark:text-white dark:hover:bg-gray-600"
        >
      </div>
    </div>

    <!-- Loading -->
    <div v-if="loading" class="flex flex-col items-center justify-center py-16">
      <div class="w-10 h-10 border-2 border-emerald-500/30 border-t-emerald-500 rounded-full animate-spin mb-4"></div>
      <p class="text-sm text-gray-400 dark:text-gray-500">Loading photos...</p>
    </div>

    <!-- Timeline items -->
    <div v-else-if="currentTimeline.length > 0" class="relative ml-4 md:ml-6 border-l-[2px] border-gray-200 space-y-8 pb-10 dark:border-gray-700 transition-colors">
      
      <div v-for="(item, index) in currentTimeline" :key="item.id" class="relative pl-6 md:pl-8">
        
        <div 
          class="absolute -left-[9px] top-1.5 w-4 h-4 rounded-full border-[3px] border-white shadow-sm ring-1 ring-gray-100 z-10 transition-colors dark:border-gray-900 dark:ring-gray-700"
          :class="index === 0 ? 'bg-emerald-500' : 'bg-gray-300 dark:bg-gray-600'"
        ></div>
        
        <div class="bg-white rounded-2xl p-4 border shadow-sm border-gray-100 hover:shadow-md hover:-translate-y-1 transition-all duration-300 dark:bg-gray-800 dark:border-gray-700">
          <div class="flex justify-between items-center mb-3">
            <span class="text-sm md:text-base font-semibold text-gray-800 dark:text-white">{{ item.timeDisplay }}</span>
          </div>
          
          <div class="rounded-xl overflow-hidden bg-gray-100 aspect-video md:aspect-[21/9] relative group dark:bg-gray-900 mb-4">
            <img :src="item.image_url" :alt="item.timeDisplay" class="w-full h-full object-cover transition-transform duration-700 group-hover:scale-105 opacity-90 group-hover:opacity-100">
          </div>

          <div class="grid grid-cols-3 gap-2">
            <div class="flex flex-col sm:flex-row items-center justify-center sm:justify-start gap-0.5 sm:gap-2 bg-gray-50 border border-gray-100 p-2 md:px-3 md:py-2 rounded-xl dark:bg-gray-900 dark:border-gray-700">
              <span class="text-[9px] md:text-[10px] font-bold text-gray-400 uppercase tracking-wider dark:text-gray-500">Soil</span>
              <span class="text-xs md:text-sm font-extrabold text-emerald-500 dark:text-emerald-400">{{ item.soil != null ? item.soil + '%' : '—' }}</span>
            </div>
            <div class="flex flex-col sm:flex-row items-center justify-center sm:justify-start gap-0.5 sm:gap-2 bg-gray-50 border border-gray-100 p-2 md:px-3 md:py-2 rounded-xl dark:bg-gray-900 dark:border-gray-700">
              <span class="text-[9px] md:text-[10px] font-bold text-gray-400 uppercase tracking-wider dark:text-gray-500">Humid</span>
              <span class="text-xs md:text-sm font-extrabold text-blue-500 dark:text-blue-400">{{ item.humid != null ? item.humid + '%' : '—' }}</span>
            </div>
            <div class="flex flex-col sm:flex-row items-center justify-center sm:justify-start gap-0.5 sm:gap-2 bg-gray-50 border border-gray-100 p-2 md:px-3 md:py-2 rounded-xl dark:bg-gray-900 dark:border-gray-700">
              <span class="text-[9px] md:text-[10px] font-bold text-gray-400 uppercase tracking-wider dark:text-gray-500">Temp</span>
              <span class="text-xs md:text-sm font-extrabold text-amber-500 dark:text-amber-400">{{ item.temp != null ? item.temp + '°C' : '—' }}</span>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Empty state -->
    <div v-else class="flex flex-col items-center justify-center py-16 px-4 text-center animate-fade-in">
      <div class="w-20 h-20 bg-gray-50 rounded-full flex items-center justify-center mb-4 border border-gray-100 shadow-inner dark:bg-gray-800 dark:border-gray-700">
        <svg class="w-10 h-10 text-gray-300 dark:text-gray-600" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 16l4.586-4.586a2 2 0 012.828 0L16 16m-2-2l1.586-1.586a2 2 0 012.828 0L20 14m-6-6h.01M6 20h12a2 2 0 002-2V6a2 2 0 00-2-2H6a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
      </div>
      <h3 class="text-gray-800 font-bold mb-1 dark:text-white">No Captures</h3>
      <p class="text-sm text-gray-500 dark:text-gray-400">There are no photos recorded for {{ formattedDate }}.</p>
    </div>

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

const formattedDate = computed(() => {
  const dateObj = new Date(selectedDate.value)
  return dateObj.toLocaleDateString('en-US', { month: 'short', day: 'numeric', year: 'numeric' })
})

const fetchPhotos = async (date) => {
  loading.value = true
  currentTimeline.value = []
  try {
    // Fetch ALL photos then filter by local date — avoids UTC timezone mismatch
    const res = await fetch(
      `${SUPABASE_URL}/rest/v1/image?select=id,image_url,created_at&order=created_at.asc`,
      { headers: { apikey: SUPABASE_KEY, Authorization: `Bearer ${SUPABASE_KEY}` } }
    )
    const data = await res.json()
    console.log('All photos from Supabase:', data)

    currentTimeline.value = (data || [])
      .filter(row => {
        // toLocaleDateString('en-CA') returns YYYY-MM-DD in local timezone
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
          temp: row.temp ?? null,
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
:global(.dark) ::-webkit-calendar-picker-indicator {
  filter: invert(1);
}
</style>