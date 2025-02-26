// 用户登录携带参数
export interface loginFormData {
    username: string
    password: string
}

//定义全部接口返回数据都拥有ts类型
export interface ResponseData {
    code: number
    message: string
}