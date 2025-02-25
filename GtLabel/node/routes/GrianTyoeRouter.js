const grianTypeContorller= require("../controlllers/GrainTypeContorller")

module.exports = app => {
    const router = require("express").Router();
    router.get("/typeInfo",grianTypeContorller.typeInfo);
    router.post("/addType",grianTypeContorller.addType);
    router.post("/deleteType",grianTypeContorller.deleteType);
    app.use('/api/type', router);
}