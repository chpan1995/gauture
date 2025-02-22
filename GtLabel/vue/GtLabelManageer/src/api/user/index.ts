import request from '@/utils/request'
import type {
    loginFormData,
    ResponseData
} from './type'

enum API {
    LOGIN_URL = '/api/user/login',
    USER_INFO_URL = '/api/user/userinfo',
    USER_COUNT_URL = '/api/user/userCount',
    ADD_USER_URL = '/api/user/addUser',
    EDIR_USER_URL = '/api/user/updateUser',
    REMOVE_USER_URL = '/api/user/removeUser'
}

// export const reqLogin = (data:loginFormData) => {
//     return request.post<any,ResponseData>(API.LOGIN_URL,data)
// }

// export const reqUsercount = ()=> {
    
// }

class UserApi {
    constructor() {
    }
    reqLogin(data:loginFormData):any {
        return request.post<any,ResponseData>(API.LOGIN_URL,data)
    }
    reqUsercount():any {
        return request.get<any>(API.USER_COUNT_URL)
    }
}

export default new UserApi;