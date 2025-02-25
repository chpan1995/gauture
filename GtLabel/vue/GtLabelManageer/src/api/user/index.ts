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

class UserApi {
    constructor() {
    }
    reqLogin(data:loginFormData):any {
        return request.post<any,ResponseData>(API.LOGIN_URL,data)
    }
    reqUsercount():any {
        return request.get<any>(API.USER_COUNT_URL)
    }
    reqUserInfo(startPos:number,endPos:number):any {
        return request.get<any>(API.USER_INFO_URL+'/'+startPos+'/'+endPos)
    }
    reqAddUser(data:any):any {
        return request.post<any,any>(API.ADD_USER_URL,data)
    }
    reqUpdateUser(data:any) {
        return request.post<any,any>(API.EDIR_USER_URL,data)
    }
    reqDeleteUser(data:any) {
        return request.post<any,any>(API.REMOVE_USER_URL,data)
    }
}

export default new UserApi;