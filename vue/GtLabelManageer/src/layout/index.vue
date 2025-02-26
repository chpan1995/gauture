<template>
    <div class="layout_container">
        <div class="layout_slider" :class="{ fold:  LayOutSettingStore.fold ? true : false }">
            <Logo></Logo>
            <el-scrollbar class="scrollbar">
                <el-menu :collapse="LayOutSettingStore.fold" :default-active="$route.path" background-color="#001529" text-color="white"
                    active-text-color="yellowgreen">
                    <Menu :menuList="userStore.menuRoutes"></Menu>
                </el-menu>
            </el-scrollbar>
        </div>

        <div class="layout_tabbar" :class="{ fold:  LayOutSettingStore.fold ? true : false }">
            <Tabbar></Tabbar>
        </div>

        <div class="layout_main" :class="{ fold:  LayOutSettingStore.fold ? true : false }" >
            <router-view v-slot="{ Component }">
                <transition name="fade">
                    <!-- 渲染layout一级路由的子路由 -->
                    <component :is="Component"/>
                </transition>
            </router-view>
        </div>
    </div>
</template>

<script setup lang="ts">
import Logo from './logo/index.vue'
//引入菜单组件
import Menu from './menu/index.vue'
import Tabbar from './tabbar/index.vue'
//获取用户相关的小仓库
import useUserStore from '@/store/modules/user';
let userStore = useUserStore();

import useLayOutSettingStore from '@/store/modules/setting';
//获取layout配置相关的仓库
let LayOutSettingStore = useLayOutSettingStore();
</script>

<style lang="scss">
.layout_container {
    width: 100%;
    height: 100vh;

    .layout_slider {
        height: 100vh;
        width: 260px;
        background-color: #001529;
        transition: all 0.3s;

        .scrollbar {
            width: 100%;
            height: calc(100vh - 50px);

            .el-menu {
                border-right: none;
            }
        }
        &.fold{
            width: 100px;
        }
    }
    .layout_tabbar {
        padding-left: 10px;
        position: fixed;
        left: 260px;
        width: calc(100% - 260px);
        height: 50px;
        transition: all 0.3s;
        top: 0px;
        display: flex;
        align-items: center;
        background: linear-gradient(to right, rgb(247, 243, 243) , rgb(250, 229, 229), rgb(245, 243, 243) );
        &.fold{
            width: calc(100vw - 100px );
            left: 100px;
        }
    }
    .layout_main{
        position: absolute;
        left:260px;
        top: 50px;
        height: calc(100% - 50px);
        width: calc(100% - 260px);
        padding: 20px;
        overflow: auto;
        transition: all 0.3s;
        &.fold{
            width: calc(100vw - 100px );
            left: 100px;
        }
    }
}
.fade-enter-from {
    opacity: 0;
    transform: scale(0);
}

.fade-enter-active {
    transition: all .3s;
}

.fade-enter-to {
    opacity: 1;
    transform: scale(1);
}
</style>