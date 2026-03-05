export const useTheme = () => {
  // สร้างตัวแปร state เก็บค่าธีม (ค่าเริ่มต้นเป็น false คือ Light Mode)
  const isDark = useState('theme', () => false)

  // ฟังก์ชันสลับโหมด (เรียกใช้เมื่อกดปุ่ม)
  const toggleTheme = () => {
    isDark.value = !isDark.value
    updateDOM()
  }

  // ฟังก์ชันอัปเดต Class HTML และบันทึกลงเครื่อง
  const updateDOM = () => {
    if (process.client) {
      const html = document.documentElement
      if (isDark.value) {
        html.classList.add('dark')
        localStorage.setItem('theme', 'dark')
      } else {
        html.classList.remove('dark')
        localStorage.setItem('theme', 'light')
      }
    }
  }

  // ฟังก์ชันเริ่มต้น (เรียกตอนเปิดเว็บ) เพื่อเช็คค่าเดิม
  const initTheme = () => {
    if (process.client) {
      const stored = localStorage.getItem('theme')
      // ถ้าเคยเลือก Dark หรือ ถ้าไม่เคยเลือกแต่เครื่องผู้ใช้เป็น Dark Mode
      if (stored === 'dark' || (!stored && window.matchMedia('(prefers-color-scheme: dark)').matches)) {
        isDark.value = true
      } else {
        isDark.value = false
      }
      updateDOM()
    }
  }

  return { isDark, toggleTheme, initTheme }
}