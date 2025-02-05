const mysql = require("mysql2");
const dbConfig = require("../config/dbConfig.js")
const logger = require('../logging/logging')

var pool = mysql.createPool({
    host: dbConfig.HOST,
    user: dbConfig.USER,
    password: dbConfig.PASSWORD,
    database: dbConfig.DB,
    port: dbConfig.PORT,
    connectionLimit: 10,
    waitForConnections: true, // 等待连接可用
});

pool.on('error', (err) => {
    logger.error('Database connection error:%s', err);
});

module.exports = pool.promise();;