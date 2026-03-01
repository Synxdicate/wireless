<template>
  <div class="antialiased">
    <NuxtLayout>
      <NuxtPage />
    </NuxtLayout>
  </div>
</template>

<script setup>
const { initTheme } = useTheme()

onMounted(() => {
  initTheme()
})

useHead({
  title: 'Smart Plant Box',
  meta: [
    // 1. บรรทัดนี้สำคัญที่สุด: บอก Browser ว่าเว็บเราทำ Dark Mode มาเองแล้ว "ไม่ต้องมายุ่ง"
    { name: 'color-scheme', content: 'light dark' },
    { name: 'theme-color', content: '#111827' } // กำหนดสีหัว Browser ให้เข้ากับธีม
  ],
  link: [
    { rel: 'icon', type: 'image/png', href: '/favicon.png' }
  ],
  htmlAttrs: {
    // กำหนด lang ให้ถูกต้อง
    lang: 'en'
  }
})
</script>

<style>
/* 2. บังคับ CSS ระดับรากฐาน */
:root {
  color-scheme: light dark;
}

/* ป้องกัน Dark Reader หรือ Browser Force Dark Mode มากลับสี */
html {
  /* สั่งให้ใช้โหมดปกติ ไม่มีการกลับสี */
  filter: none !important; 
}

/* เมื่อเข้า Dark Mode ให้บังคับว่าเป็น Dark จริงๆ และพื้นหลังต้องมืด */
html.dark {
  color-scheme: dark;
  background-color: #111827; /* สี gray-900 */
  color: #f3f4f6; /* สี gray-100 */
}

/* แก้ไข Scrollbar ให้สวยงามและเปลี่ยนสีตามโหมด */
::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}
::-webkit-scrollbar-track {
  background: transparent;
}
::-webkit-scrollbar-thumb {
  background-color: #cbd5e1; /* สี Scrollbar ตอน Light */
  border-radius: 4px;
}
html.dark ::-webkit-scrollbar-thumb {
  background-color: #374151; /* สี Scrollbar ตอน Dark */
}
</style>