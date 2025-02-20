const pool = require("./db.js")
const logger = require('../logging/logging')

let usermodel = new Object();

// 用户登录
usermodel.login = async (username, password) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT id, password FROM user WHERE username=?', [username]);
        if (rows.length > 0) {
            console.log(rows[0].password);
            if(rows[0].password === password)
                return rows[0];
        }
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

module.exports = usermodel;