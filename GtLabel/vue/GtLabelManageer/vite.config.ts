import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import path from 'path' // npm install --save-dev @types/node
// https://vite.dev/config/
export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
        "@": path.resolve("./src") // 相对路径别名配置，使用 @ 代替 src
    }
 }
})
