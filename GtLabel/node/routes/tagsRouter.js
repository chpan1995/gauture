const tagContorller= require("../controlllers/tagsContorller")

module.exports = app => {
    const router = require("express").Router();
    router.post("/tagInfo",tagContorller.tagInfo);
    router.post("/addTas",tagContorller.addTas);
    router.post("/deleteTags",tagContorller.deleteTags);
    router.get("/tags",tagContorller.tags);
    app.use('/api/tags', router);
}