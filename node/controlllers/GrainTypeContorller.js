const grainTypeModel = require('../models/GrainTypeModel')

exports.typeInfo = (req, res) => {
    (async () => {
        let data = await grainTypeModel.typeInfo();
        if (data) {
            res.send({
                code: 200,
                message: "获取谷物种类",
                data: data
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while typeInfo."
            });
        }
    })();
}

exports.addType = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await grainTypeModel.addType(req.body.name);
        if (data) {
            res.status(200).send({
                code:200,
                message:"添加谷物种类成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while addType."
            });
        }
    })();
}

exports.deleteType = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await grainTypeModel.deleteType(req.body.id);
        if (data) {
            res.status(200).send({
                code:200,
                message:"删除谷物种类成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while deleteType."
            });
        }
    })();
}