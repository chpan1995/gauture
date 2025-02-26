const tagsModel = require('../models/tagsModel')

exports.tagInfo = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await tagsModel.tagInfo(req.body.typeid,req.body.deep);
        if (data) {
            res.status(200).send({
                code:200,
                message:"获取标签信息成功",
                data:data
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while tagInfo."
            });
        }
    })();
}


exports.addTas = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await tagsModel.addTas(req.body);
        if (data) {
            res.status(200).send({
                code:200,
                message:"添加标签信息成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while addTas."
            });
        }
    })();
}

exports.deleteTags = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await tagsModel.deleteTags(req.body.id);
        if (data) {
            res.status(200).send({
                code:200,
                message:"删除标签信息成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while deleteTags."
            });
        }
    })();
}

exports.tags = (req, res) => {
    (async()=>{
        const data = await tagsModel.tags();
        if(data) {
            res.send({
                code: 200,
                message: "get tags success",
                data:data
            });
        } else {
            res.status(500).send({
                code: 500,
                message: "Some error occurred while tags."
            });
        }
    })();
}