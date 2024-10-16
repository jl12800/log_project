#ifndef _M_FORMAT_H__
#define _M_FORMAT_H__

#include "level.hpp"
#include "message.hpp"
#include <memory>
#include <ctime>
#include <vector>
#include <cassert>
#include <sstream>
#include <stdlib.h>

namespace jl12800
{
    // 抽象格式化子项基类
    class FORMAT_ITEM
    {
    public:
        using ptr = std::shared_ptr<FORMAT_ITEM>;
        virtual void format(std::ostream &out, const LOG_MSG &msg) = 0;
    };

    // 派生类格式化子项--消息、等级、时间、文件名、行号、线程ID、日志器名、制表符、换行、其他
    class MsgFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << msg._payload;
        }
    };

    class LevelFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << LOG_LEVEL::tostring(msg._level);
        }
    };

    class TimeFormatItem : public FORMAT_ITEM
    {
    public:
        TimeFormatItem(const std::string &fmt = "%H:%M:%S") : _time_fmt(fmt) {}
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            struct tm t;
            localtime_r(&msg._ctime, &t);
            char tmp[32] = {0};
            strftime(tmp, 31, _time_fmt.c_str(), &t);

            out << tmp;
        }

    private:
        std::string _time_fmt; //%H:%M:%S
    };

    class FileFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << msg._file;
        }
    };

    class LineFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << msg._line;
        }
    };

    class ThreadFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << msg._logger;
        }
    };

    class TabFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << "\t";
        }
    };

    class NlineFormatItem : public FORMAT_ITEM
    {
    public:
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << "\n";
        }
    };

    class OtherFormatItem : public FORMAT_ITEM
    {
    public:
        OtherFormatItem(const std::string &str) : _str(str) {}
        void format(std::ostream &out, const LOG_MSG &msg) override
        {
            out << _str;
        }

    private:
        std::string _str;
    };

    /*  %d 日期        %T  缩进     %t 线程ID   %p 日志级别
        %c 日志器名称   %f 文件名    %l 行号     %m 日志消息    %n 换行
    */
    class FORMATTER
    {
    public:
        using ptr = std::shared_ptr<FORMATTER>;

        // 提供默认的日志输出格式：[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n
        FORMATTER(const std::string &pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
            : _pattern(pattern)
        {
            assert(parsePattern());
        }

        // 对msg进行格式化，辅助std::string format(const LOG_MSG &msg)
        void format(std::ostream &out, const LOG_MSG &msg)
        {
            // _items 是一个包含所有格式化子项的向量，代表了用户定义的日志格式化规则的具体实现
            for (auto &item : _items)
            {
                item->format(out, msg); // 对每一个格式化子项调用其 format 函数，并输出到 out
            }
        }

        // msg格式化输出至stringstream ss
        std::string format(const LOG_MSG &msg)
        {
            std::stringstream ss; // 创建一个字符串流对象 ss
            format(ss, msg);      // 内部调用 format(std::ostream &out, const Log_Msg &msg)，并将 ss 作为输出流传递给它
            // 当内部的 format(std::ostream &out, const LogMsg &msg) 函数执行完毕后，ss 中已经包含了完整的格式化日志内容
            return ss.str(); // 最终通过 ss.str() 将字符串流中的内容转换为 std::string 并返回。
        } /*它创建了一个 std::stringstream 对象 ss，用于临时存储格式化后的日志内容。*/

        /* 关键点总结
            执行顺序：
                先调用外部的 std::string format(const LogMsg &msg)。
                外部函数内部调用 void format(std::ostream &out, const LogMsg &msg)。
                内部函数执行完毕后，外部函数返回最终格式化的字符串。
            作用：
                外部函数提供了一个简便的接口来获取格式化后的日志字符串。
                内部函数负责具体的格式化逻辑，将日志信息按照定义好的规则输出到指定的流（在这个例子中是 std::stringstream）。
            灵活性：
                内部函数使用 std::ostream 作为参数，这使得它可以被用于输出到不同的目标，比如文件流、网络流等，不仅限于字符串流。  */
    private:
        // 对格式化规则字符串进行解析，获取std::vector<std::pair<std::string, std::string>>
        bool parsePattern()
        {
            // 1. 对格式化规则字符串解析
            std::vector<std::pair<std::string, std::string>> fmt_order;
            size_t pos = 0;
            std::string key, val;

            while (pos < _pattern.size())
            {
                // 1. 处理原始字符串
                // 1.1 判断是%吗，若不是就说明是原始字符，存入val
                if (_pattern[pos] != '%')
                {
                    val.push_back(_pattern[pos++]);
                    continue;
                }

                // 1.2 走到这里说明pos位置就是%字符。判断是不是%%，如果是则插入val
                if (pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')
                {
                    val.push_back('%');
                    pos += 2;
                    continue;
                }

                // 1.3 当val中存在数据时，即非空；说明上面两步有数据被存入val，原始字符串处理完毕，对应key设为空
                if (val.empty() == false)
                {
                    fmt_order.push_back(std::make_pair("", val)); // 组成第一个key,val；其中val是原始字符串
                    val.clear();
                }

                // 2. 这里说明val中为空，即pos位置是%，且pos+1位置不是%
                pos += 1; // pos->格式化字符，如d

                // 2.1 判断pos是否走到字符串末尾，若是说明%之后，没有对应的格式化字符串
                if (pos == _pattern.size()) // 这里说明pos+1位置不存在
                {
                    std::cout << "%之后，没有对应的格式化字符串！\n";
                    return false;
                }

                // 2.2 此时pos是格式化字符，如d，将pos位置的字符存入key
                key = _pattern[pos]; // %d{%H%M%S},此时pos=d

                // 2.3 进一步判断是不是带有子项{%H%M%S}。pos+1是不是{
                pos += 1;

                // 2.4 如果pos是{  ，就处理可能含有的字串
                if (pos < _pattern.size() && _pattern[pos] == '{') // 没有走到字符串末尾，且pos是{
                {
                    pos += 1; // 这时候pos->'{'之后的位置，%H%M%S

                    // 2.4.1 循环将{}内部的数据存入val
                    while (pos < _pattern.size() && _pattern[pos] != '}')
                        val.push_back(_pattern[pos++]);

                    // 2.4.2 pos走到了末尾跳出循环，代表没有遇到'}'即格式是错误的
                    if (pos == _pattern.size())
                    {
                        std::cout << "子规则匹配出错！\n";
                        return false;
                    }

                    // 2.4.3 没有走到末尾跳出循环，说明下一个字符是'}'
                    pos += 1;
                }

                // 2.4.4 获取含有子项的格式化字符
                fmt_order.push_back(std::make_pair(key, val));
                key.clear();
                val.clear();
            }

            // 2. 根据解析得到的数据初始化格式化子项数组成员，调用FORMAT_ITEM::ptr creatItem(const std::string &key, const std::string &val)
            for (auto &it : fmt_order)
            {
                _items.push_back(creatItem(it.first, it.second));
            }
            return true;
        }

        // 根据不同的格式化字符创建不同的格式化子项
        FORMAT_ITEM::ptr creatItem(const std::string &key, const std::string &val)
        {
            if (key == "d")
                return std::make_shared<TimeFormatItem>(val);
            if (key == "t")
                return std::make_shared<ThreadFormatItem>();
            if (key == "c")
                return std::make_shared<LoggerFormatItem>();
            if (key == "f")
                return std::make_shared<FileFormatItem>();
            if (key == "l")
                return std::make_shared<LineFormatItem>();
            if (key == "p")
                return std::make_shared<LevelFormatItem>();
            if (key == "T")
                return std::make_shared<TabFormatItem>();
            if (key == "m")
                return std::make_shared<MsgFormatItem>();
            if (key == "n")
                return std::make_shared<NlineFormatItem>();
            if (key == "")
                return std::make_shared<OtherFormatItem>(val);
            std::cout << "没有对应的格式化字符：%" << key << std::endl;
            abort();
            return FORMAT_ITEM::ptr();
        }

    private:
        std::string _pattern;                 // 格式化规则子项
        std::vector<FORMAT_ITEM::ptr> _items; // 存储解析后的格式化子项
    };
}
#endif