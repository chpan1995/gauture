<template>
    <el-icon style="margin-right: 10px;">
        <component :is="LayOutSettingStore.fold? 'Fold' : 'Expand' " @click="change()"></component>
    </el-icon>
    <el-breadcrumb separator-icon="ArrowRight">
        <template v-for="(item,index) in $route.matched" :key="index">
            <el-breadcrumb-item v-if="item.meta.title" :to="item.path">
                <el-icon>
                    <component :is="item.meta.icon"></component>
                </el-icon>
                <span style="margin-left: 4px;">{{item.meta.title}}</span>
            </el-breadcrumb-item>
        </template>
    </el-breadcrumb>
</template>

<script setup lang="ts">
import useLayOutSettingStore from '@/store/modules/setting';
//获取layout配置相关的仓库
let LayOutSettingStore = useLayOutSettingStore();
import { useRoute } from 'vue-router';
//获取路由对象
let $route = useRoute();

const change = ()=>{
    LayOutSettingStore.fold=!LayOutSettingStore.fold
}

</script>

<style scoped lang="scss">

</style>