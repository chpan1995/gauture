const userModel = require("../models/userModel")

exports.login = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    const { username, password } = req.body;
    userModel.login(username, password).then(data => {
        if (data) {
            res.send({
                code: 200,
                message: "login success",
                userid: data.id,
                nickname:data.nickname
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while login."
            });
        }
    });
}

exports.userCount = (req, res) => {
    (async () => {
        let data = await userModel.userCount();
        if (data) {
            res.send({
                code: 200,
                message: "获取用户数量",
                count: data
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while userCount."
            });
        }
    })();
}

exports.addUser = (req, res) => {
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await userModel.addUser(req.body);
        if (data) {
            res.status(200).send({
                code:200,
                message:"添加用户信息成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while adduser."
            });
        }
    })();
}

exports.updateUser = (req,res)=>{
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await userModel.updateUser(req.body);
        if (data) {
            res.status(200).send({
                code:200,
                message:"更新用户信息成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while updateUser."
            });
        }
    })();
}

exports.removeUser = (req,res)=>{
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async () => {
        let data = await userModel.removeUser(req.body.id);
        if (data) {
            res.status(200).send({
                code:200,
                message:"删除用户信息成功"
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while removeUser."
            });
        }
    })();
}

exports.userinfo = (req,res) => {
    (async () => {
        let page = parseInt(req.params.page) || 0; // 获取页码
        let pageSize = parseInt(req.params.pageSize) || 0; // 获取每页数量
        let data = await userModel.userinfo(page,pageSize);
        if (data) {
            res.status(200).send({
                code:200,
                message:"获取用户信息成功",
                data:data
            });
        } else {
            res.status(200).send({
                code: 201,
                message: "Some error occurred while userinfo."
            });
        }
    })();
}