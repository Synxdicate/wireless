<template>
  <div class="max-w-2xl mx-auto pb-20 relative">
    
    <header class="flex items-center mb-8 relative">
      <NuxtLink to="/" class="absolute left-0 text-gray-500 hover:text-emerald-600 p-2 transition-colors dark:text-gray-400 dark:hover:text-emerald-400">
        <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7"></path></svg>
      </NuxtLink>
      <h1 class="text-xl font-bold text-gray-800 w-full text-center dark:text-white transition-colors">Settings & Automation</h1>
    </header>

    <div class="space-y-6 px-2">

      <section class="bg-gray-100/80 rounded-3xl p-6 dark:bg-gray-800/80 transition-colors border border-gray-100 dark:border-gray-700">
        <h2 class="text-base font-bold text-gray-800 mb-1 dark:text-white">Watering Schedule</h2>
        <p class="text-xs text-gray-500 mb-6 dark:text-gray-400">Set up automatic watering times or control it manually.</p>
        
        <div class="space-y-4">
          <div @click="wateringSchedule = 'manual'" class="flex flex-col sm:flex-row sm:items-center justify-between gap-2 sm:gap-4 cursor-pointer group bg-white dark:bg-gray-900 p-4 rounded-2xl border border-gray-100 dark:border-gray-700 hover:border-emerald-300 dark:hover:border-emerald-500/50 transition-all">
            <div class="flex items-center gap-3 shrink-0">
              <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors" :class="wateringSchedule === 'manual' ? 'border-emerald-600 dark:border-emerald-400' : 'border-gray-300 dark:border-gray-600'">
                <div v-if="wateringSchedule === 'manual'" class="w-2.5 h-2.5 bg-emerald-600 dark:bg-emerald-400 rounded-full"></div>
              </div>
              <span class="text-sm font-bold text-gray-700 dark:text-gray-200">Manual Only</span>
            </div>
            <div class="ml-8 sm:ml-0 flex sm:justify-end">
              <span class="text-[10px] text-gray-400 dark:text-gray-500 uppercase font-bold tracking-wider">Button Press</span>
            </div>
          </div>

          <div @click="wateringSchedule = '1'" class="flex flex-col sm:flex-row sm:items-center justify-between gap-3 sm:gap-4 cursor-pointer group bg-white dark:bg-gray-900 p-4 rounded-2xl border border-gray-100 dark:border-gray-700 hover:border-emerald-300 dark:hover:border-emerald-500/50 transition-all">
            <div class="flex items-center gap-3 shrink-0">
              <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors" :class="wateringSchedule === '1' ? 'border-emerald-600 dark:border-emerald-400' : 'border-gray-300 dark:border-gray-600'">
                <div v-if="wateringSchedule === '1'" class="w-2.5 h-2.5 bg-emerald-600 dark:bg-emerald-400 rounded-full"></div>
              </div>
              <span class="text-sm font-bold text-gray-700 dark:text-gray-200">1 time / day</span>
            </div>
            <div class="ml-8 sm:ml-0 flex sm:justify-end">
              <input 
                type="time" 
                v-model="time1"
                @click.stop 
                :disabled="wateringSchedule !== '1'"
                class="w-full sm:w-[140px] min-w-0 text-xs font-bold bg-gray-50 border border-gray-200 text-gray-700 px-3 py-2 sm:py-1.5 rounded-xl dark:bg-gray-800 dark:border-gray-600 dark:text-gray-300 outline-none focus:ring-2 focus:ring-emerald-500 focus:border-emerald-500 transition-all disabled:opacity-50 disabled:cursor-not-allowed cursor-pointer text-center"
              />
            </div>
          </div>

          <div @click="wateringSchedule = '2'" class="flex flex-col sm:flex-row sm:items-center justify-between gap-3 sm:gap-4 cursor-pointer group bg-white dark:bg-gray-900 p-4 rounded-2xl border border-gray-100 dark:border-gray-700 hover:border-emerald-300 dark:hover:border-emerald-500/50 transition-all">
            <div class="flex items-center gap-3 shrink-0">
              <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center transition-colors" :class="wateringSchedule === '2' ? 'border-emerald-600 dark:border-emerald-400' : 'border-gray-300 dark:border-gray-600'">
                <div v-if="wateringSchedule === '2'" class="w-2.5 h-2.5 bg-emerald-600 dark:bg-emerald-400 rounded-full"></div>
              </div>
              <span class="text-sm font-bold text-gray-700 dark:text-gray-200">2 times / day</span>
            </div>
            <div class="ml-8 sm:ml-0 flex items-center justify-between sm:justify-end gap-2 overflow-hidden">
              <input 
                type="time" 
                v-model="time1"
                @click.stop 
                :disabled="wateringSchedule !== '2'"
                class="flex-1 sm:flex-none sm:w-[110px] min-w-0 text-xs font-bold bg-gray-50 border border-gray-200 text-gray-700 px-2 py-2 sm:py-1.5 rounded-xl dark:bg-gray-800 dark:border-gray-600 dark:text-gray-300 outline-none focus:ring-2 focus:ring-emerald-500 transition-all disabled:opacity-50 disabled:cursor-not-allowed cursor-pointer text-center"
              />
              <span class="text-gray-400 text-[10px] font-bold shrink-0">&</span>
              <input 
                type="time" 
                v-model="time2"
                @click.stop 
                :disabled="wateringSchedule !== '2'"
                class="flex-1 sm:flex-none sm:w-[110px] min-w-0 text-xs font-bold bg-gray-50 border border-gray-200 text-gray-700 px-2 py-2 sm:py-1.5 rounded-xl dark:bg-gray-800 dark:border-gray-600 dark:text-gray-300 outline-none focus:ring-2 focus:ring-emerald-500 transition-all disabled:opacity-50 disabled:cursor-not-allowed cursor-pointer text-center"
              />
            </div>
          </div>
        </div>
      </section>

      <section class="bg-gray-100/80 rounded-3xl p-6 dark:bg-gray-800/80 transition-colors border border-gray-100 dark:border-gray-700">
        <h2 class="text-base font-bold text-gray-800 mb-1 dark:text-white">Auto Photo Capture</h2>
        <p class="text-xs text-gray-500 mb-6 dark:text-gray-400">How often should the camera take a photo for the timeline?</p>
        
        <div class="space-y-4">
          <label v-for="item in photoOptions" :key="item.value" class="flex items-center gap-3 cursor-pointer group bg-white dark:bg-gray-900 p-4 rounded-2xl border border-gray-100 dark:border-gray-700 hover:border-emerald-300 dark:hover:border-emerald-500/50 transition-all">
            <input type="radio" :value="item.value" v-model="photoFrequency" class="hidden">
            <div class="w-5 h-5 rounded-full border-[1.5px] flex items-center justify-center shrink-0 transition-colors" :class="photoFrequency === item.value ? 'border-emerald-600 dark:border-emerald-400' : 'border-gray-300 dark:border-gray-600'">
              <div v-if="photoFrequency === item.value" class="w-2.5 h-2.5 bg-emerald-600 dark:bg-emerald-400 rounded-full"></div>
            </div>
            <div class="flex flex-col">
              <span class="text-sm font-bold text-gray-700 dark:text-gray-200">{{ item.label }}</span>
              <span v-if="item.desc" class="text-[10px] text-gray-400 dark:text-gray-500 mt-0.5">{{ item.desc }}</span>
            </div>
          </label>
        </div>
      </section>
      
    </div>

    <div class="fixed z-[100] flex items-center gap-2 bg-gray-800 text-white px-5 py-2.5 rounded-full text-sm shadow-xl transition-all duration-300 bottom-24 left-1/2 -translate-x-1/2 md:bottom-10 md:left-auto md:right-10 md:translate-x-0" :class="showToast ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-4 pointer-events-none'">
      <span v-if="isSaving" class="w-4 h-4 border-2 border-white/30 border-t-white rounded-full animate-spin"></span>
      <svg v-else class="w-4 h-4 text-emerald-400" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7"></path></svg>
      {{ isSaving ? 'Saving...' : 'Settings saved' }}
    </div>

  </div>
</template>

<script setup>
import { ref, watch, onMounted } from 'vue'
import { ref as dbRef, get, update } from 'firebase/database' 
import { db } from '~/utils/firebase'

useHead({ title: 'Settings | Smart Plant Box' })

const wateringSchedule = ref('manual')       
const photoFrequency   = ref('morning') 
const time1            = ref('07:00') 
const time2            = ref('17:00') 

const isSaving  = ref(false)
const showToast = ref(false)
let toastTimeout   = null
let isInitialLoad  = true

const photoOptions = [
  { label: 'Manual Only',              value: 'manual',               desc: 'Take photos manually via Dashboard' },
  { label: 'Morning',                  value: 'morning',              desc: '1 photo per day (07:00 AM)' }, 
  { label: 'Morning & Evening',        value: 'morning_evening',      desc: '2 photos per day (07:00 AM & 12:00 PM)' }, 
  { label: 'Morning, Noon & Evening',  value: 'morning_noon_evening', desc: '3 photos per day (07:00 AM & 12:00 PM & 5:00 PM)' }
]

// =====================================================
// LOAD SETTINGS
// =====================================================
onMounted(async () => {
  try {
    const snapshot = await get(dbRef(db, 'test/settings'))
    if (snapshot.exists()) {
      const data = snapshot.val()
      if (data.wateringSchedule) wateringSchedule.value = data.wateringSchedule
      if (data.photoFrequency)   photoFrequency.value   = data.photoFrequency
      if (data.time1)            time1.value            = data.time1
      if (data.time2)            time2.value            = data.time2
    }
  } catch (error) {
    console.error('Error loading settings:', error)
  } finally {
    setTimeout(() => { isInitialLoad = false }, 500)
  }
})

// =====================================================
// AUTO SAVE
// =====================================================
const autoSave = async () => {
  if (isInitialLoad) return

  isSaving.value  = true
  showToast.value = true
  clearTimeout(toastTimeout)

  try {
    // เซฟ UI settings
    await update(dbRef(db, 'test/settings'), {
      wateringSchedule: wateringSchedule.value,
      photoFrequency:   photoFrequency.value,
      time1:            time1.value,
      time2:            time2.value
    })

    // ✅ sync ไปที่ path ที่ ESP32 อ่าน
    await update(dbRef(db, 'test/schedule'), {
      mode:  wateringSchedule.value,  // "manual", "1", "2"
      time1: time1.value,             // "07:00"
      time2: time2.value              // "17:00"
    })

  } catch (error) {
    console.error('Error saving settings:', error)
  }

  isSaving.value = false
  toastTimeout   = setTimeout(() => { showToast.value = false }, 2000)
}

watch([wateringSchedule, photoFrequency, time1, time2], autoSave)
</script>

<style scoped>
.animate-fade-in { 
  animation: fadeIn 0.3s ease-out; 
}
@keyframes fadeIn { 
  from { opacity: 0; transform: translateY(-10px); } 
  to   { opacity: 1; transform: translateY(0); } 
}
input[type="time"]::-webkit-calendar-picker-indicator {
  cursor: pointer;
  opacity: 0.6;
  transition: 0.2s;
}
input[type="time"]::-webkit-calendar-picker-indicator:hover {
  opacity: 1;
}
:global(.dark) input[type="time"]::-webkit-calendar-picker-indicator {
  filter: invert(1);
}
</style>