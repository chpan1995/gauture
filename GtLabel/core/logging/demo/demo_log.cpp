/*
 * @File: 
 * @Author: jun.wang wangjun@gaozhe.com.cn
 * @Brief: 
 * @Date: 2023-07-25 14:19:55
 * @LastEditors: wang jun
 * @LastEditTime: 2023-07-27 14:48:52
 * 
 * Copyright (c) 2023 All Rights Reserved.
 */
#include "logging/log.h"
#include <QApplication>


int main(int argc,char** argv) {
    QApplication a(argc, argv);
    qDebug() << "test 1";
    logging::log_info(DL,"test 2");
    return a.exec();
}
