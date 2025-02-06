const winston = require('winston')
require('winston-daily-rotate-file')

// 自定义日志格式
const logFormat = winston.format.combine(
    winston.format.timestamp({
        format: 'YYYY-MM-DD HH:mm:ss'
    }),
    winston.format.errors({ stack: true }),
    winston.format.splat(),
    //   winston.format.json()
    winston.format.printf(({ level, message, timestamp }) => {
        return `${timestamp} [${level.toUpperCase()}]: ${message}`;
    })
)

// 创建 logger 实例
const logger = winston.createLogger({
    level: 'info',
    format: logFormat,
    transports: [
        // 控制台输出
        new winston.transports.Console({
            format: logFormat
        }),

        // 普通信息日志文件
        new winston.transports.DailyRotateFile({
            filename: 'logs/app-%DATE%.log',
            datePattern: 'YYYY-MM-DD',
            zippedArchive: true,
            maxSize: '20m',
            maxFiles: '14d',
            level: 'info',
            format: logFormat
        })
    ]
})

// // 处理未捕获的异常
// logger.exceptions.handle(
//     new winston.transports.DailyRotateFile({
//         filename: 'logs/exceptions-%DATE%.log',
//         datePattern: 'YYYY-MM-DD',
//         zippedArchive: true,
//         maxSize: '20m',
//         maxFiles: '14d',
//         format: logFormat
//     })
// )

// // 处理未处理的 Promise 拒绝
// logger.rejections.handle(
//     new winston.transports.DailyRotateFile({
//         filename: 'logs/rejections-%DATE%.log',
//         datePattern: 'YYYY-MM-DD',
//         zippedArchive: true,
//         maxSize: '20m',
//         maxFiles: '14d', // 日志保留时间（maxFiles）
//         format: logFormat
//     })
// )
module.exports = logger;