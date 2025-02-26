const userContorller= require("../controlllers/userContorller")

module.exports = app => {
    const router = require("express").Router();
    router.post("/login",userContorller.login);
    router.get("/userCount",userContorller.userCount);
    router.post("/addUser",userContorller.addUser);
    router.post("/updateUser",userContorller.updateUser);
    router.post("/removeUser",userContorller.removeUser);
    router.get("/userinfo/:page/:pageSize",userContorller.userinfo);
    app.use('/api/user', router);
}