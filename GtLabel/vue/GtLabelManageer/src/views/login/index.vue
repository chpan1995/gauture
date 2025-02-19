<template>
    <div class="login_container">
        <div class="container">
            <el-form class="login_form" ref="ruleFormRef" :rules="rules" :model="loginForm" label-width="auto"
                :label-position="labelPosition">
                <h1>欢迎来到高哲标注系统后台</h1>
                <el-form-item label="用户名" prop="username">
                    <el-input v-model="loginForm.username"></el-input>
                </el-form-item>
                <el-form-item label="密码" prop="password">
                    <el-input type="password" v-model="loginForm.password"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button :loading="loading" class="login_btn" @click="login" type="primary"
                        size="default">登录</el-button>
                </el-form-item>
            </el-form>
        </div>
    </div>
</template>

<script setup lang="ts">
import { reactive, ref } from "vue";
import type { FormRules } from 'element-plus'
import { ElNotification } from 'element-plus';
import { useRouter,useRoute } from "vue-router";
import useUserStore from "@/store/modules/user"

let userStore = useUserStore();
//获取路由器
let $router = useRouter();
//路由对象 
let $route = useRoute();

// 获取el-from组件
const ruleFormRef = ref()
//定义变量控制按钮加载效果
let loading = ref(false);
const labelPosition = ref<any>("right");
// 收集账号密码
let loginForm = reactive({
    username: "",
    password: ""
})

const validatorUserName = (rule: any, value: any, callback: any) => {
    if (value.length >= 2) {
        callback()
    } else {
        callback(new Error('账号长度至少2位'));
    }
}

const validatorUserPasswd = (rule: any, value: any, callback: any) => {
    if (value.length >= 5) {
        callback()
    } else {
        callback(new Error('账号长度至少5位'));
    }
}

const rules = reactive<FormRules<typeof loginForm>>({
    username: [{ validator: validatorUserName, trigger: 'change' }],
    password: [{ validator: validatorUserPasswd, trigger: 'change' }]
})

const login = async () => {
    // 保证全部表单相校验通过再发请求
    await ruleFormRef.value.validate();
    //加载效果:开始加载
    loading.value = true;
    try {
        await userStore.userLogin(loginForm);
        //登录成功加载效果也消失
        loading.value = false;
        $router.push('/layout');
    } catch (error) {
        //登录失败加载效果消息
        loading.value = false;
        //登录失败的提示信息
        ElNotification({
            type: 'error',
            message: (error as Error).message
        })
    }
}

</script>

// 由于你使用了 scoped,默认情况下,子组件的样式会被限制在当前组件范围内,
// 而 Element Plus 的 el-form-item 的 label 是其内部渲染的,不能直接被 scoped 样式影响。
<style lang="scss">
.login_container {
    width: 100%;
    height: 100vh;
    background: url("@/assets/loginbk.jpg") no-repeat;
    background-position: center;
    /* 背景图片居中显示 */
    background-size: cover;
    /* 图片按比例拉伸以覆盖整个区域 */
    display: flex;
    /* 启用 flex 布局 */
    justify-content: center;
    /* 水平居中 */
    align-items: center;

    /* 垂直居中 */
    .container {
        position: relative;

        width: 30%;
        padding: 30px;
        background: url("@/assets/login_form.png");

        .login_btn {
            width: 100%;
        }

        .el-form-item {
            margin-bottom: 25px;
            /* 设置每行之间的间距 */
        }

        label {
            color: white !important;
            font-size: 18px !important;
        }

        h1 {
            color: white;
            font-size: 24px;
            margin: 20px 0px;
            text-align: center;
            /* 水平居中 */
            margin: 0px 0px 30px 0px;
        }
    }
}
</style>