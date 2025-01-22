const userContorller= require("../controlllers/userContorller")

module.exports = app => {
    const router = require("express").Router();
    router.post("/login",userContorller.login);
    app.use('/api/user', router);
}