<template>
  <div class="bg-gray-50 min-h-screen flex font-sans text-gray-800 overflow-x-hidden transition-colors duration-300 dark:bg-gray-900 dark:text-gray-100">
    
    <aside 
      class="hidden md:flex flex-col h-screen fixed bg-white border-r border-gray-100 shadow-sm z-40 transition-all duration-300 ease-in-out overflow-hidden whitespace-nowrap dark:bg-gray-800 dark:border-gray-700"
      :class="isSidebarOpen ? 'w-64' : 'w-20'"
    >
      <div class="h-24 flex items-center px-5 gap-3 shrink-0">
         <button @click="isSidebarOpen = !isSidebarOpen" class="p-2 text-gray-400 hover:text-emerald-600 hover:bg-emerald-50 rounded-xl transition-colors shrink-0 focus:outline-none dark:hover:bg-gray-700 dark:hover:text-emerald-400">
           <svg v-if="!isSidebarOpen" class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16"></path></svg>
           <svg v-else class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h10M4 18h16"></path></svg>
         </button>
         <div class="flex items-center gap-2 transition-all duration-300" :class="isSidebarOpen ? 'opacity-100 translate-x-0' : 'opacity-0 -translate-x-4 pointer-events-none w-0'">
           <img src="/favicon.png" class="w-8 h-8 object-contain shrink-0" />
           <span class="text-xl font-bold text-emerald-600 dark:text-emerald-400">Smart Plant</span>
         </div>
      </div>

      <nav class="flex-1 px-3 space-y-2 mt-2">
         <NuxtLink to="/" class="flex items-center gap-3 py-3 text-gray-600 hover:bg-emerald-50 hover:text-emerald-600 rounded-xl transition-all overflow-hidden dark:text-gray-400 dark:hover:bg-gray-700 dark:hover:text-emerald-400" exact-active-class="bg-emerald-50 text-emerald-600 font-semibold dark:bg-gray-700 dark:text-emerald-400" :class="isSidebarOpen ? 'px-4 justify-start' : 'px-0 justify-center'">
            <svg class="w-6 h-6 shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2V6zM14 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2V6zM4 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2v-2zM14 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2v-2z"></path></svg>
            <span class="transition-opacity duration-300" :class="isSidebarOpen ? 'opacity-100' : 'opacity-0 w-0 hidden'">Dashboard</span>
         </NuxtLink>
         <NuxtLink to="/timeline" class="flex items-center gap-3 py-3 text-gray-600 hover:bg-emerald-50 hover:text-emerald-600 rounded-xl transition-all overflow-hidden dark:text-gray-400 dark:hover:bg-gray-700 dark:hover:text-emerald-400" active-class="bg-emerald-50 text-emerald-600 font-semibold dark:bg-gray-700 dark:text-emerald-400" :class="isSidebarOpen ? 'px-4 justify-start' : 'px-0 justify-center'">
            <svg class="w-6 h-6 shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
            <span class="transition-opacity duration-300" :class="isSidebarOpen ? 'opacity-100' : 'opacity-0 w-0 hidden'">Timeline</span>
         </NuxtLink>
         <NuxtLink to="/settings" class="flex items-center gap-3 py-3 text-gray-600 hover:bg-emerald-50 hover:text-emerald-600 rounded-xl transition-all overflow-hidden dark:text-gray-400 dark:hover:bg-gray-700 dark:hover:text-emerald-400" active-class="bg-emerald-50 text-emerald-600 font-semibold dark:bg-gray-700 dark:text-emerald-400" :class="isSidebarOpen ? 'px-4 justify-start' : 'px-0 justify-center'">
            <svg class="w-6 h-6 shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 002.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 001.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 00-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 00-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 00-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 00-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 001.066-2.573c-.94-1.543.826-3.31 2.37-2.37.996.608 2.296.07 2.572-1.065z"></path><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z"></path></svg>
            <span class="transition-opacity duration-300" :class="isSidebarOpen ? 'opacity-100' : 'opacity-0 w-0 hidden'">Settings</span>
         </NuxtLink>
      </nav>
      </aside>

    <main 
      class="flex-1 w-full pb-20 md:pb-0 transition-all duration-300 ease-in-out"
      :class="isSidebarOpen ? 'md:ml-64' : 'md:ml-20'"
    >
      <div class="max-w-7xl mx-auto p-4 md:p-8 pt-8 md:pt-10">
        <slot />
      </div>
    </main>

    <nav class="md:hidden fixed bottom-0 w-full bg-white border-t border-gray-100 flex justify-around px-2 py-3 pb-6 z-50 shadow-[0_-4px_20px_-1px_rgba(0,0,0,0.05)] dark:bg-gray-800 dark:border-gray-700">
       <NuxtLink to="/" class="flex flex-col items-center p-2 text-gray-400 transition-colors dark:text-gray-500" exact-active-class="!text-emerald-600 drop-shadow-sm dark:!text-emerald-400">
         <svg class="w-6 h-6 mb-1" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2V6zM14 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2V6zM4 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2v-2zM14 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2v-2z"></path></svg>
         <span class="text-[10px] font-bold">Dashboard</span>
       </NuxtLink>
       <NuxtLink to="/timeline" class="flex flex-col items-center p-2 text-gray-400 transition-colors dark:text-gray-500" exact-active-class="!text-emerald-600 drop-shadow-sm dark:!text-emerald-400">
         <svg class="w-6 h-6 mb-1" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z"></path></svg>
         <span class="text-[10px] font-bold">Timeline</span>
       </NuxtLink>
       <NuxtLink to="/settings" class="flex flex-col items-center p-2 text-gray-400 transition-colors dark:text-gray-500" exact-active-class="!text-emerald-600 drop-shadow-sm dark:!text-emerald-400">
         <svg class="w-6 h-6 mb-1" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 002.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 001.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 00-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 00-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 00-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 00-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 001.066-2.573c-.94-1.543.826-3.31 2.37-2.37.996.608 2.296.07 2.572-1.065z"></path><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 12a3 3 0 11-6 0 3 3 0 016 0z"></path></svg>
         <span class="text-[10px] font-bold">Settings</span>
       </NuxtLink>
       </nav>
  </div>
</template>

<script setup>
import { ref } from 'vue'
const isSidebarOpen = ref(true)
// ตัด useTheme() ออกจากที่นี่ เพราะไม่มีปุ่มให้กดแล้ว
</script>