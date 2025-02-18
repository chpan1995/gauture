<template>
    <div class="logo" v-if="setting.logoHidden">
        <div class="logoimg"></div>
        <p v-show="!LayOutSettingStore.fold"> {{setting.title}} </p>
    </div>
</template>

<script lang="ts" setup>
import setting from '@/setting';
import useLayOutSettingStore from '@/store/modules/setting';
//获取layout配置相关的仓库
let LayOutSettingStore = useLayOutSettingStore();
</script>

<style lang="scss">
.logo{
    width: 100%;
    height: 50px;
    display: flex;
    align-items: center;
    justify-content: center;
    /* 水平居中 */
    padding: 10px;
    .logoimg{
        width: 40px;
        height: 40px;
        background: url('@/assets/icon.png') no-repeat;
        background-position: center;
        background-size: cover;
        /* 图片按比例拉伸以覆盖整个区域 */
    }
    p{
       font-size: 20px; 
       margin-left: 10px;
       color: white;
    }
}

</style>