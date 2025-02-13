// 创建用户相关小仓库
import { defineStore } from "pinia"
import { reqLogin } from '@/api/user'
import type {
    loginFormData, ResponseData
  } from '@/api/user/type'
const useUserStore = defineStore('User',{
    // 小仓库储存数据地方
    state:() => {
        return {
            token:''
        }
    },
    // 处理异步逻辑地方
    actions: {
        async userLogin(data:loginFormData){
            const result:ResponseData =await reqLogin(data);
            if (result.code == 200) {
                //pinia仓库存储一下token
                //由于pinia|vuex存储数据其实利用js对象
                this.token = result.message as string
                //能保证当前async函数返回一个成功的promise
                return 'ok'
              } else {
                return Promise.reject(new Error(result.message))
              }
        }
    },
    getters:{}
}) 

export default useUserStore;