const pool = require("./db.js")
const logger = require('../logging/logging.js')

let GrainTypeModel = new Object();

GrainTypeModel.typeInfo = async () => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT id ,name FROM graintype', []);
        return rows;
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

GrainTypeModel.addType = async (name) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('INSERT INTO graintype(name) VALUES(?)', [name]);
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

GrainTypeModel.deleteType = async (id) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('DELETE FROM graintype WHERE id = ?', [id]);
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

module.exports = GrainTypeModel;