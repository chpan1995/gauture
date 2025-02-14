// 对外暴露路由
export const constantRoute = [
    {
        // 登录路由
        path: "/login",
        component: () => import('@/views/login/index.vue'),
        name: 'login', // 命名路由
    },
    {
        path: "/",
        redirect:"/login",
        // component: () => import('@/views/home/index.vue'),
        name: 'layout',
    },
    {
        path: '/404',
        component: () => import('@/views/404/index.vue'),
        name: '404',
    },
    {
        // 重定向
        path:'/:pathMatch(.*)*',
        redirect:'/404',
        name:'any'
    }
] 