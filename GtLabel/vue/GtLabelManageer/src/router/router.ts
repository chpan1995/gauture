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
        path: "/cig",
        component: () => import('@/layout/index.vue'),
        name: 'Cig',
        redirect: '/cig/graintype',
        meta: {
            title: '标注信息设置',
            hidden: false,
            icon: 'User',
        },
        children:[
            {
                path:'/cig/graintype',
                component: () => import('@/views/labconfig/graintype/index.vue'),
                meta: {
                    title: '谷物种类配置',
                    hidden: false,
                    icon: 'User',
                  },
            },
            {
                path:'/cig/toplevel',
                component: () => import('@/views/labconfig/toplevel/index.vue'),
                meta: {
                    title: '一级分类配置',
                    hidden: false,
                    icon: 'User',
                  },
            },
            {
                path:'/cig/firstOrderTag',
                component: () => import('@/views/labconfig/firstOrderTag/index.vue'),
                meta: {
                    title: '分类配置1配置',
                    hidden: false,
                    icon: 'User',
                  },
            },
            {
                path:'/Cig/secOrderTag',
                component: () => import('@/views/labconfig/secOrderTag/index.vue'),
                meta: {
                    title: '分类配置2配置',
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