const pool = require("./db.js")
const logger = require('../logging/logging')

let tagsModel = new Object();

function mergeData(rows) {
    const map = new Map();

    for (const { id, chrildobj } of rows) {
        const objList = JSON.parse(chrildobj);

        for (const obj of objList) {
            if (!map.has(obj.value)) {
                // 如果是第一次出现，存入 map
                map.set(obj.value, [{ value: obj.value, id, items: [...obj.items] }]);
            } else {
                let existingList = map.get(obj.value);

                if (existingList.length === 1) {
                    // 如果目前只有一条，创建第二条
                    existingList.push({ value: obj.value, id, items: [...obj.items] });
                } else {
                    // 如果已经有两条了，合并到第二条
                    const existingItems = new Set(existingList[1].items.map(item => item.value));
                    for (const item of obj.items) {
                        if (!existingItems.has(item.value)) {
                            existingList[1].items.push(item);
                        }
                    }
                }
            }
        }
    }

    return [].concat(...map.values());
}
tagsModel.tagInfo = async (typeid, deep) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT id,chrildobj FROM graintags WHERE deep=? AND typeid=?', [deep, typeid]);
        return mergeData(rows);
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

tagsModel.addTas = async (data) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('INSERT INTO graintags(chrildobj,deep,typeid) VALUES(?,?,?)', [data.chrildobj, data.deep, data.typeid]);
        if (result.affectedRows > 0) return true;
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

tagsModel.deleteTags = async (id) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('DELETE  FROM graintags WHERE id = ?', [id]);
        if (result.affectedRows > 0) return true;
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

module.exports = tagsModel;