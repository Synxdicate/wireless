import { initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';

// กุญแจ Firebase ของคุณ
const firebaseConfig = {
  apiKey: "AIzaSyC4CEMA9XeWvv92l1nwl2IP9hCn99ffJrk",
  authDomain: "project-wireless-3e13c.firebaseapp.com",
  databaseURL: "https://project-wireless-3e13c-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "project-wireless-3e13c",
  storageBucket: "project-wireless-3e13c.firebasestorage.app",
  messagingSenderId: "693301774811",
  appId: "1:693301774811:web:b6949977491a3d956ebffe"
};

// สั่งให้ระบบเริ่มต่อเน็ตไปหา Firebase
export const firebaseApp = initializeApp(firebaseConfig);

// ส่งตัว Database ออกไปให้หน้า Dashboard ของเราเรียกใช้ได้
export const db = getDatabase(firebaseApp);