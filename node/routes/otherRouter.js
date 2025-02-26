const otherContorller= require("../controlllers/otherContorller")

module.exports = app => {
    const router = require("express").Router();
    router.get("/update",otherContorller.update);
    app.use('/api', router);
}