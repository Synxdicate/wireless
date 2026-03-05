import type { Config } from 'tailwindcss'

export default <Config>{
  // บรรทัดนี้สำคัญที่สุด คือบอกให้ใช้ Class 'dark' ในการสลับโหมด
  darkMode: 'class', 
  
  content: [
    // ระบุไฟล์ที่จะให้ Tailwind ทำงาน
    "./components/**/*.{js,vue,ts}",
    "./layouts/**/*.vue",
    "./pages/**/*.vue",
    "./plugins/**/*.{js,ts}",
    "./app.vue",
    "./error.vue",
  ],
  theme: {
    extend: {},
  },
  plugins: [],
}