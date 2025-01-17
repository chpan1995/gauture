/*
 * @File: log.h
 * @Author: chenghao.pan@gaozhe.com.cn
 * @Brief: 
 * @Date: 2024-06-24 14:46:28
 * @LastEditTime: 2024-06-24 14:44:56
 * 
 * Copyright (c) 2024 All Rights Reserved.
 */


#ifndef LOGGING_LOG_H_
#define LOGGING_LOG_H_

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/details/null_mutex.h"
#include "spdlog/details/registry.h"

#ifndef SPDLOG_USE_STD_FORMAT
  #include "spdlog/fmt/ranges.h"
#endif

#define DL logging::logger::instance()->m_mutilLog
#define RL logging::logger::instance()->m_rotalog

namespace logging {

class logger {
private:
  logger() { init(); }
public:
    static logger* instance();
    ~logger();
    void init();
    std::shared_ptr<spdlog::logger> m_rotalog;
    std::shared_ptr<spdlog::logger> m_mutilLog;
};


template<typename LG,typename... T>
inline void log_trace(LG&& log,T&&... msg){
    log->trace(std::forward<T>(msg)...);
}

template<typename LG,typename... T>
inline void log_info(LG&& log,T&&... msg){
    log->info(std::forward<T>(msg)...);
}

template<typename LG,typename... T>
inline void log_debug(LG&& log,T&&... msg){
    log->debug(std::forward<T>(msg)...);
}

template<typename LG,typename... T>
inline void log_warn(LG&& log,T&&... msg){
    log->warn(std::forward<T>(msg)...);
}

template<typename LG,typename... T>
inline void log_error(LG&& log,T&&... msg){
    log->error(std::forward<T>(msg)...);
}

template<typename LG,typename... T>
inline void log_critical(LG&& log,T&&... msg){
    log->critical(std::forward<T>(msg)...);
}

}
#endif  // LOGGING_LOG_H_
