import request from '@/utils/request'

enum API {
    TAG_INFO_URL = '/api/tags/tagInfo',
    ADD_TAG_URL = '/api/tags/addTas',
    DELETE_TAG_URL = '/api/tags/deleteTags',
}

class TagsApi {
    constructor() {
    }
    reqTagsInfo(data:any):any {
        return request.post<any>(API.TAG_INFO_URL,data)
    }
    reqAddTags(data:any):any {
        return request.post<any,any>(API.ADD_TAG_URL,data)
    }
    reqDeleteTags(data:any):any {
        return request.post<any,any>(API.DELETE_TAG_URL,data)
    }
}

export default new TagsApi;