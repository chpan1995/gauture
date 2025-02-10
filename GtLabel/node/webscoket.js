const WebSocket = require('ws');
const url = require('url');
const logger = require('./logging/logging')


// 创建 WebSocket 服务
const wss = new WebSocket.Server({ port: 8081 });

// 用于存储所有客户端连接
const clients = new Map();

logger.info('WebSocket server is running on ws://localhost:8081');

// 监听客户端连接
wss.on('connection', (ws, request) => {
    const queryParams = new URLSearchParams(url.parse(request.url).query);
    logger.log(queryParams);
    const clientId = queryParams.get("username");
    const keys = clients.keys();
    let findkey = '';
    for (const key of keys) {
        if (key.includes(clientId.split("_")[0])) {
            findkey = key;
            break;
        }
    }
    if (findkey !== '') {
        // 发送退出登录的信息
        let obj = { "commond": "logout" };
        sendToClient(findkey, obj);
        iscnt = true;
        clients.get(findkey).close();
        clients.delete(findkey);
        logger.info("关闭上次的连接");
    }
    clients.set(clientId, ws);
    logger.info(`Client connected: ${clientId}`);
    // 监听客户端消息
    ws.on('message', (message) => {
        handleClientMessage(clientId, message);
    });

    // 监听客户端断开连接
    ws.on('close', () => {
        clients.delete(clientId);
        logger.info(`Client disconnected: ${clientId}`);
    });

    // 错误处理
    ws.on('error', (error) => {
        logger.error(`Error from ${clientId}:`, error.message);
    });
});

// 处理客户端发送的消息
function handleClientMessage(clientId, message) {
    try {
        let obj = JSON.parse(message.toString());
        if (obj["commond"] === "keepAlive") {
            clients.get(clientId).send(`${message}`);
        }
        logger.info(`recive ${clientId} message: ${message}`);
    } catch (error) {
        logger.log(error);
    }
}

// 向指定客户端发送消息
function sendToClient(clientId, message) {
    const client = clients.get(clientId);
    if (client) {
        client.send(JSON.stringify(message));
    } else {
        logger.warn(`Client ${clientId} not found.`);
    }
}


