import { createApp } from 'vue'
import '@/styles/index.scss'
import App from './App.vue'
import  ElementPlus  from 'element-plus'
import 'element-plus/dist/index.css'
import zhCn from "element-plus/es/locale/lang/zh-cn"

// 引入路由
import router from './router'
// 引入仓库
import pinia from './store'


const app = createApp(App);
// 注册模板路由
app.use(router);
// 安装仓库
app.use(pinia);

app.use(
    ElementPlus,
    {
        locale:zhCn
    }
)
// 组件挂载到DOM
app.mount('#app');