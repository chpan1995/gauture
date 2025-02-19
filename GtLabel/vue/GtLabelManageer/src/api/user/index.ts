import request from '@/utils/request'
import type {
    loginFormData,
    ResponseData
} from './type'

enum API {
    LOGIN_URL = '/api/user/login',
}

export const reqLogin = (data:loginFormData) => {
    return request.post<any,ResponseData>(API.LOGIN_URL,data)
}