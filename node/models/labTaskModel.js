const pool = require("./db.js")
const logger = require('../logging/logging')

const MAX_RETRIES = 3; // 最大重试次数

let labTaskmodel = new Object();

// 获取未完成的任务
labTaskmodel.labTaskList = async (userid) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT imgcnt,uploadcnt,taskname,taskid,createtime FROM labtask WHERE userid=? and status=0 ORDER BY createtime ASC', [userid]);
        if (rows.length > 0) {
            return rows;
        }
        return [];
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 获取未完成的任务的图片信息
labTaskmodel.labTaskInfo = async (taskid) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT imgname,tags FROM labtaskinfo WHERE taskid=?', [taskid]);
        if (rows.length > 0) {
            return rows;
        }
        return [];
    } catch (err) {
        logger.error(err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 添加任务
labTaskmodel.addLabTask = async (userid, taskname, imgarry) => {
    let connection;
    let attempt = 0;
    while (attempt < MAX_RETRIES) {
        try {
            attempt++; // 增加尝试次数
            connection = await pool.getConnection();
            await connection.beginTransaction(); // 开始事务
            await connection.query('INSERT INTO labtask (userid,imgcnt,taskname) VALUES (?,?,?)', [userid, imgarry.length, taskname]);

            const [rows] = await connection.query('SELECT taskid FROM labtask WHERE userid = ? ORDER BY taskid DESC LIMIT 1', [userid]);

            let data = [];
            for (const img of imgarry) {
                let it = [rows[0].taskid];
                it.push(img);
                data.push(it);
            }

            await connection.query('INSERT INTO labtaskinfo (taskid,imgname) VALUES ?', [data]);
            await connection.commit(); // 提交事务
            logger.info('Transaction completed successfully');
            return rows;
        } catch (err) {
            if (connection) await connection.rollback(); // 回滚事务
            // 检查是否是死锁错误
            if (err.code === 'ER_LOCK_DEADLOCK') {
                logger.warn(`Deadlock detected, retrying... Attempt ${attempt}/${MAX_RETRIES}`);
                // 添加延迟避免立即重试
                await new Promise(resolve => setTimeout(resolve, 100 * attempt));
                // 如果是死锁错误，重试
                if (attempt === MAX_RETRIES) {
                    logger.error('Deadlock retry limit reached. Giving up.');
                }
            } else {
                logger.error(err);
                attempt = 100;
            }
        } finally {
            if (connection) connection.release(); // 释放连接
        }
    }
}

// 更新标注信息
labTaskmodel.updateTags = async (taskid, imgname, tags) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [result] = await connection.query('UPDATE labtaskinfo set tags =? WHERE taskid=? AND imgname=?', [tags, taskid, imgname]);
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

// 上传标注信息(删除labtaskinfo里面上传图片，存到taskinfohistory里去,再更新labtask里面的uploadcnt值)
labTaskmodel.labupload = async (arrData) => {
    let connection;
    let attempt = 0;
    while (attempt < MAX_RETRIES) {
        try {
            attempt++; // 增加尝试次数
            connection = await pool.getConnection();
            await connection.beginTransaction(); // 开始事务
            // arrData [[1,"111.png","tags"]]
            await connection.query('INSERT INTO labtaskhistory (taskid,imgname,tags) VALUES ?', [arrData]);

            let parm = [];
            for (const it of arrData) {
                parm.push(it[1]);
            }
            let sql = `
        DELETE FROM labtaskinfo
        WHERE taskid = ? AND ${parm
                    .map(() => ' imgname = ? ')
                    .join(' OR ')}
        `;

            await connection.query(sql, [arrData[0][0], ...parm]);

            const [rows] = await connection.query('SELECT imgcnt, uploadcnt FROM labtask WHERE taskid = ?', [arrData[0][0]]);

            let status = false;
            if (arrData.length + rows[0].uploadcnt === rows[0].imgcnt) {
                status = true;
            }
            await connection.query('UPDATE labtask set uploadcnt = ? , status=? WHERE taskid=?', [arrData.length + rows[0].uploadcnt, status, arrData[0][0]]);

            await connection.commit(); // 提交事务
            logger.info('Transaction completed successfully');
            return true;
        } catch (err) {
            if (connection) await connection.rollback(); // 回滚事务
            // 检查是否是死锁错误
            if (err.code === 'ER_LOCK_DEADLOCK') {
                logger.warn(`Deadlock detected, retrying... Attempt ${attempt}/${MAX_RETRIES}`);
                // 添加延迟避免立即重试
                await new Promise(resolve => setTimeout(resolve, 100 * attempt));
                // 如果是死锁错误，重试
                if (attempt === MAX_RETRIES) {
                    logger.error('Deadlock retry limit reached. Giving up.');
                }
            } else {
                logger.error(err);
                attempt = 100;
            }
        } finally {
            if (connection) connection.release(); // 释放连接
        }
    }
}

module.exports = labTaskmodel;