import request from '@/utils/request'
import type {
    loginFormData,
    ResponseData
} from './type'

enum API {
    LOGIN_URL = '/user/login',
}

export const reqLogin = (data:loginFormData) => {
    console.log(1111);
    return request.post<any,ResponseData>(API.LOGIN_URL,data)
}