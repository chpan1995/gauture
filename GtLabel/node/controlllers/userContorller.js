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
                userid:data.id
            });
        } else {
            res.status(500).send({
                code: 500,
                message: "Some error occurred while login."
            });
        }
    });
}