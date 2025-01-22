const pool = require("./db.js")

let labTaskmodel = new Object();

// 获取未完成的任务
labTaskmodel.labTaskList = async (userid) => {
    let connection;
    try {
        // 从连接池获取连接
        connection = await pool.getConnection();
        // 执行查询
        const [rows] = await connection.query('SELECT imgcnt,uploadcnt,taskname,taskid,createtime FROM labtask WHERE userid=? ORDER BY createtime ASC', [userid]);
        if (rows.length > 0) {
            return rows;
        }
        return [];
    } catch (err) {
        console.error('Error:', err);
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
        const [rows] = await connection.query('SELECT imgname,tags FROM labtaskinfo WHERE taskid=? AND upload=false', [taskid]);
        if (rows.length > 0) {
            return rows;
        }
        return [];
    } catch (err) {
        console.error('Error:', err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 添加任务
labTaskmodel.addLabTask = async (userid, taskname, imgarry) => {
    let connection;
    try {
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
        console.log('Transaction completed successfully');
        return rows;
    } catch (err) {
        if (connection) await connection.rollback(); // 回滚事务
        console.error('Transaction failed:', err);
    } finally {
        if (connection) connection.release(); // 释放连接
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
        console.error('Error:', err);
    } finally {
        // 确保连接被释放
        if (connection) connection.release();
    }
}

// 上传标注信息(删除labtaskinfo里面上传图片，存到taskinfohistory里去,再更新labtask里面的uploadcnt值)
labTaskmodel.upload = async (arrData) => {
    let connection;
    try {
        connection = await pool.getConnection();
        await connection.beginTransaction(); // 开始事务
        // arrData [[1,"tags","111.png"]]
        await connection.query('INSERT INTO labtaskhistory (taskid,imgname,tags) VALUES ?', [arrData]);

        let parm = [];
        for (const it of arrData) {
            parm.push(it[1]);
        }
        let sql =  `
        DELETE FROM labtaskinfo
        WHERE taskid = ? ${parm
          .map(() => '(imgname = ?)')
          .join(' OR ')}
        `;

        await connection.query(sql, [arrData[0][0],parm]);

        await connection.query('UPDATE labtask set uploadcnt = ? WHERE taskid=?', [arrData.length]);

        await connection.commit(); // 提交事务
        console.log('Transaction completed successfully');
        return rows;
    } catch (err) {
        if (connection) await connection.rollback(); // 回滚事务
        console.error('Transaction failed:', err);
    } finally {
        if (connection) connection.release(); // 释放连接
    }
}

module.exports = labTaskmodel;