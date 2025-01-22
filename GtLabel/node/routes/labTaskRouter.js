const labTaskContorller = require("../controlllers/labTaskContorller")

module.exports = app => {
    const router = require("express").Router();

    // 获取未完成的任务
    router.post("/labTaskList", labTaskContorller.labTaskList);
    // 获取未完成的任务的图片信息
    router.post("/labTaskInfo", labTaskContorller.labTaskInfo);
    // 添加任务
    router.post("/addLabTask", labTaskContorller.addLabTask);
    // 更新标注信息
    router.post("/updateTags", labTaskContorller.updateTags);

    app.use('/api/labTask', router);
}