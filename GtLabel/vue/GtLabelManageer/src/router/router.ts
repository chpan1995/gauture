// 对外暴露路由
export const constantRoute = [
    {
        // 登录路由
        path: "/login",
        component: () => import('@/views/login/index.vue'),
        name: 'login', // 命名路由
        meta: {
            title: '',
            hidden: true,
            icon: '',
        },
    },
    {
        path: "/",
        redirect:"/login",
        name: '/',
        meta: {
            title: '',
            hidden: true,
            icon: '',
        },
    },
    {
        path: "/layout",
        component: () => import('@/layout/index.vue'),
        name: 'layout',
        meta: {
            title: '',
            hidden: false,
            icon: '',
        },
        redirect: '/home',
        children:[
            {
                path:'/home',
                component: () => import('@/views/home/index.vue'),
                meta: {
                    title: '首页',
                    hidden: false,
                    icon: 'HomeFilled',
                  },
            }
        ]
    },

    {
        path: "/acl",
        component: () => import('@/layout/index.vue'),
        name: 'Acl',
        redirect: '/acl/user',
        meta: {
            title: '',
            hidden: false,
            icon: '',
        },
        children:[
            {
                path:'/acl/user',
                component: () => import('@/views/user/index.vue'),
                meta: {
                    title: '用户管理',
                    hidden: false,
                    icon: 'User',
                  },
            }
        ]
    },

    {
        path: '/404',
        component: () => import('@/views/404/index.vue'),
        name: '404',
        meta: {
            title: '',
            hidden: true,
            icon: '',
        },
    },
    {
        // 重定向
        path:'/:pathMatch(.*)*',
        redirect:'/404',
        name:'any',
        meta: {
            title: '',
            hidden: true,
            icon: '',
        },
    }
] 