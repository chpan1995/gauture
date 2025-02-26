import request from '@/utils/request'

enum API {
    TYPE_INFO_URL = '/api/type/typeInfo',
    ADD_TYPE_URL = '/api/type/addtype',
    DELETE_TYPE_URL = '/api/type/deleteType',
}

class GrainTypeApi {
    constructor() {
    }
    reqTypeInfo():any {
        return request.get<any>(API.TYPE_INFO_URL)
    }
    reqAddType(data:any):any {
        return request.post<any,any>(API.ADD_TYPE_URL,data)
    }
    reqDeleteType(data:any):any {
        return request.post<any,any>(API.DELETE_TYPE_URL,data)
    }
}

export default new GrainTypeApi;