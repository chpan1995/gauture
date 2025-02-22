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
            if (rows[0].password === password)
                return rows[0];
        }
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 获取用户数量
usermodel.userCount = async () => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT COUNT(*) as count FROM user', []);
        return rows[0]['count'];
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 获取所有用户信息
usermodel.userinfo = async (startPos, endPos) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT id,username,password,nickname,createtime FROM user limit ?,?', [startPos, endPos]);
        return rows;
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 添加新用户
usermodel.addUser = async (data) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('INSERT INTO user(username,password,nickname) VALUES(?,?,?)'
            , [data.username, data.password, data.nickname]);
        if (result.affectedRows > 0) {
            return true;
        }
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 更新用户信息
usermodel.updateUser = async (data) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('UPDATE user SET username = ?,password = ?,nickname = ? WHERE id = ?'
            , [data.username, data.password, data.nickname, data.id]);
        if (result.affectedRows > 0) {
            return true;
        }
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 删除指定用户
usermodel.removeUser = async(id) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('DELETE FROM user WHERE id = ?', [id]);
        if (result.affectedRows > 0) {
            return true;
        }
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

module.exports = usermodel;