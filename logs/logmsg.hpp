#pragma once

#include "level.hpp"
#include "utill.hpp"
#include <thread>

namespace jl12800
{
    class LOGMSG
    {
    public:
        LOGMSG(LOGLEVEL::value level, size_t line, const std::string file, const std::string logger, const std::string msg)
            : _ctime(utill::DATE::now()),
              _level(level), _line(line),
              _tid(std::this_thread::get_id()),
              _file(file), _logger(logger),
              _payload(msg) {}

    public:
        time_t _ctime;          // 日志产生的时间戳
        LOGLEVEL::value _level; // 日志等级
        size_t _line;           // 行号
        std::thread::id _tid;   // 线程ID
        std::string _file;      // 源码文件名
        std::string _logger;    // 日志器名称
        std::string _payload;   // 有效消息数据
    };
}