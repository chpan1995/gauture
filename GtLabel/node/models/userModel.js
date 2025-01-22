const pool = require("./db.js")

let usermodel = new Object();

// 用户登录
usermodel.login = async (username, password) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT password FROM user WHERE username=?', [username]);
        if (rows.length > 0) {
            rows[0].password === password;
            return true;
        }
    } catch (err) {
        console.error('Error:', err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

module.exports = usermodel;