#pragma once
#include <iostream>
#include <string>

namespace jl12800
{
    // 1. LogLevel 类用于定义日志系统的日志等级
    class LOGLEVEL
    {
    public:
        // 1.1 枚举类 value 定义了日志等级
        enum class value
        {
            UNKNOW = 0, // 未知等级，通常表示未初始化或无法识别的日志等级
            DEBUG,      // 调试信息，主要用于开发调试
            INFO,       // 一般信息，表示正常操作的信息
            WARN,       // 警告信息，表明存在潜在问题但程序仍然可以运行
            ERROR,      // 错误信息，表明程序遇到了问题，可能导致功能不可用
            FATAL,      // 致命错误，表明程序遇到了无法恢复的严重错误，通常需要立即终止
            OFF         // 关闭日志输出，表示不输出任何日志信息
        };

        // 1.2 将日志等级枚举值转换为对应的字符串表示
        static const char *tostring(LOGLEVEL::value level) {
            // 使用 switch 语句匹配不同的日志等级，并返回对应的字符串
            switch (level) {
                case LOGLEVEL::value::DEBUG:
                    return "DEBUG"; // 当日志等级为 DEBUG 时返回 "DEBUG"
                case LOGLEVEL::value::INFO:
                    return "INFO"; // 当日志等级为 INFO 时返回 "INFO"
                case LOGLEVEL::value::WARN:
                    return "WARN"; // 当日志等级为 WARN 时返回 "WARN"
                case LOGLEVEL::value::ERROR:
                    return "ERROR"; // 当日志等级为 ERROR 时返回 "ERROR"
                case LOGLEVEL::value::FATAL:
                    return "FATAL"; // 当日志等级为 FATAL 时返回 "FATAL"
                case LOGLEVEL::value::OFF:
                    return "OFF"; // 当日志等级为 OFF 时返回 "OFF"
            }
            // 如果没有匹配到任何已知日志等级，返回 "UNKNOW"
            return "UNKNOW";
        }
    };
}