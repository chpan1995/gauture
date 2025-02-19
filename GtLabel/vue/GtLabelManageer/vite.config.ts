import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';
import path from 'path';
import { loadEnv } from 'vite'

// https://vitejs.dev/config/
export default defineConfig(({ command, mode  }) => {
  let env = loadEnv(mode, process.cwd())
  return {
    plugins: [
      vue(),
  
    ],
    resolve: {
      alias: {
        "@": path.resolve("./src") // 相对路径别名配置，使用 @ 代替 src
      },
    },

    server: {
      proxy: {
        [env.VITE_APP_BASE_API]: {
          //获取数据服务器地址的设置
          target: env.VITE_SERVE,
          //需要代理跨域
          changeOrigin: true,
          //路径重写
          rewrite: (path) => path.replace(/^\/api/, ''),
        },
      },
    }
  }
});
