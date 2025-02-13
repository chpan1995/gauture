import { createApp } from 'vue'
import '@/styles/index.scss'
import App from './App.vue'
import './style.css'
import  ElementPlus  from 'element-plus'
import 'element-plus/dist/index.css'
import zhCn from "element-plus/es/locale/lang/zh-cn"

const app = createApp(App);

app.use(
    ElementPlus,
    {
        locale:zhCn
    }
)
// 组件挂载到DOM
app.mount('#app');