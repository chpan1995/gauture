import{r as e}from"./request-Bboo2TiE.js";class t{constructor(){}reqLogin(r){return e.post("/api/user/login",r)}reqUsercount(){return e.get("/api/user/userCount")}reqUserInfo(r,s){return e.get("/api/user/userinfo/"+r+"/"+s)}reqAddUser(r){return e.post("/api/user/addUser",r)}reqUpdateUser(r){return e.post("/api/user/updateUser",r)}reqDeleteUser(r){return e.post("/api/user/removeUser",r)}}const p=new t;export{p as u};
