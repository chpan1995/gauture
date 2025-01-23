const labTaskModel = require("../models/labTaskModel")

exports.labTaskList = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    labTaskModel.labTaskList(req.body.userid).then(
        data => {
            if (data) {
                res.send({
                    code: 200,
                    labtask: data
                });
            } else {
                res.status(500).send({
                    code: 500,
                    message: "Some error occurred while labTaskList."
                });
            }
        }
    );
}

exports.labTaskInfo = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    labTaskModel.labTaskInfo(req.body.taskid).then(
        data => {
            if (data) {
                res.send({
                    code: 200,
                    labtaskinfo: data
                });
            } else {
                res.status(500).send({
                    code: 500,
                    message: "Some error occurred while labTaskInfo."
                });
            }
        }
    );
}

exports.addLabTask = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    const { userid, taskname, imgarry } = req.body;
    labTaskModel.addLabTask(userid, taskname, imgarry).then(
        data => {
            if (data) {
                res.send({
                    code: 200,
                    labTaskid: data[0].taskid
                });
            } else {
                res.status(500).send({
                    code: 500,
                    message: "Some error occurred while addLabTask."
                });
            }
        }
    );
}

exports.updateTags = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    const { taskid, imgname, tags } = req.body;
    labTaskModel.updateTags(taskid, imgname, tags).then(
        data => {
            if (data === true) {
                res.send({
                    code: 200,
                    message: "add updateTags success"
                });
            } else {
                res.status(500).send({
                    code: 500,
                    message: "Some error occurred while updateTags."
                });
            }
        }
    );
}

exports.labupload = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            code: 400,
            message: "Content can not be empty!"
        });
        return;
    }
    (async()=>{
        const data = await labTaskModel.labupload(req.body);
        if(data) {
            res.send({
                code: 200,
                message: "add labupload success"
            });
        } else {
            res.status(500).send({
                code: 500,
                message: "Some error occurred while labupload."
            });
        }
    })();
}