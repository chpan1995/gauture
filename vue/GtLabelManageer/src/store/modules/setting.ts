import { defineStore } from 'pinia'
const LayOutSettingStore = defineStore('setting',{
    state:() => {
        return {
            fold:false
        }
    },
    actions:{},
    getters:{}
});
export default LayOutSettingStore;