const mysql = require("mysql2");
const dbConfig = require("../config/dbConfig.js")
const logger = require('../logging/logging')

var pool = mysql.createPool({
    host: process.env.DATABASE_HOST || dbConfig.HOST,
    user: process.env.DB_USER || dbConfig.USER,
    password: process.env.DB_PASSWORD || dbConfig.PASSWORD,
    database: process.env.DB_NAME || dbConfig.DB,
    port: process.env.DB_PORT || dbConfig.PORT,
    connectionLimit: 10,
    waitForConnections: true, // 等待连接可用
});

pool.on('error', (err) => {
    logger.error('Database connection error:%s', err);
});

module.exports = pool.promise();;