/*
 * @File: log.h
 * @Author: chenghao.pan@gaozhe.com.cn
 * @Brief:
 * @Date: 2024-06-24 14:46:28
 * @LastEditTime: 2024-12-10 14:04:28
 *
 * Copyright (c) 2024 All Rights Reserved.
 */

#include "log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace logging {


logger *logger::instance()
{
    static logger it;
    return &it;
}

logger::~logger()
{
    // spdlog::shutdown();
}

void logger::init() {
    // 获取当前可执行文件的路径
    std::filesystem::path execPath;
#ifdef _WIN32
    // Windows 方式
    wchar_t path[1024];
    GetModuleFileNameW(NULL, path, 1024);
    execPath = path;
#else
    execPath = std::filesystem::canonical("/proc/self/exe");
#endif
    // 获取所在目录
    auto execDir = execPath.parent_path();
    // 创建log存放文件夹
    auto log_path = execDir.string()+"/logs";

    if (std::filesystem::exists(log_path)) {
        std::filesystem::create_directories(log_path);
    }
    // 天数
    auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_path+"/day.log",2,20);
    daily_sink->set_level(spdlog::level::trace);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    std::vector<spdlog::sink_ptr> sinks{daily_sink, console_sink};
    m_mutilLog = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(),sinks.end());
    m_mutilLog->set_level(spdlog::level::trace);
    m_mutilLog->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%L%$] %v");
    spdlog::details::registry::instance().register_logger(m_mutilLog);

    // 滚动日志
    auto rota_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_path+"/rotating.log",1048576 * 50, 3);
    rota_sink->set_level(spdlog::level::trace);
    auto console_sink_r = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink_r->set_level(spdlog::level::trace);
    console_sink_r->set_pattern(u8"[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    std::vector<spdlog::sink_ptr> sinks_r{rota_sink, console_sink_r};
    m_rotalog = std::make_shared<spdlog::logger>("multi_sink_2", sinks_r.begin(),sinks_r.end());
    spdlog::details::registry::instance().register_logger(m_rotalog);
    m_rotalog->set_level(spdlog::level::trace);

    spdlog::flush_every(std::chrono::milliseconds(10));
}

}  // namespace logging

