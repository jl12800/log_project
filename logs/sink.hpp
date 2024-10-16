#ifndef __M_SINK_H__
#define __M_SINK_H__

#include "utill.hpp"
#include <memory>
#include <fstream>
#include <cassert>
#include <sstream>

namespace jl12800
{
    class LOG_SINK
    {
    public:
        using ptr = std::shared_ptr<LOG_SINK>;
        LOG_SINK() {}
        virtual ~LOG_SINK() {}
        virtual void log(const char *data, size_t len) = 0;
    };

    // 落地方向：标准输出
    class StdoutSink : public LOG_SINK
    {
    public:
        // 将日志消息写入到标准输出
        void log(const char *data, size_t len)
        {
            std::cout.write(data, len);
        }
    };

    // 落地方向：指定文件---要知道放哪个文件---私有成员：文件名
    class FileSink : public LOG_SINK
    {
    public:
        // 构造时传入文件名，并打开文件，将操作句柄管理起来
        FileSink(const std::string &pathname) : _pathname(pathname)
        {
            // 1. 创建日志文件所在的目录
            utill::FILE::create_directory(utill::FILE::filepath(pathname));

            // 2. 创建并打开日志文件
            _ofs.open(_pathname, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        // 将日志消息写入到指定文件
        void log(const char *data, size_t len)
        {
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string _pathname;
        std::ofstream _ofs;
    };

    // 落地方向：滚动文件（以大小进行滚动）
    // 负责按文件大小滚动日志文件的日志接收器
    class RollBySizeSink : public LOG_SINK
    {
    public:
        // 构造函数，接受基础文件名和最大文件大小作为参数
        RollBySizeSink(const std::string &basename, size_t max_size)
            : _basename(basename), _max_fsize(max_size), _cur_fsize(0), _name_count(0)
        {
            // 创建新的日志文件并获取其路径
            std::string pathname = creatNewFile();
            // 1. 创建日志文件所在的目录
            utill::FILE::create_directory(utill::FILE::filepath(pathname));
            // 2. 创建并打开日志文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);
            // 确保文件成功打开
            assert(_ofs.is_open());
        }

        // 将日志消息写入到文件中
        void log(const char *data, size_t len)
        {
            // 检查当前文件大小是否超过最大文件大小
            if (_cur_fsize >= _max_fsize)
            {
                // 1. 关闭当前文件
                _ofs.close();
                // 2. 创建新的文件名
                std::string pathname = creatNewFile();
                // 3. 打开新的日志文件
                _ofs.open(pathname, std::ios::binary | std::ios::app);
                // 4. 确保新文件成功打开
                assert(_ofs.is_open());
                // 5. 重置当前文件大小
                _cur_fsize = 0;
            }

            // 写入日志数据到当前文件
            _ofs.write(data, len);

            // 检查写入操作是否成功
            assert(_ofs.good());

            // 更新当前文件的已写入大小
            _cur_fsize += len;
        }

    private:
        // 创建新的日志文件名
        std::string creatNewFile()
        {
            // 获取系统时间
            time_t t = utill::DATE::now();
            struct tm lt;
            localtime_r(&t, &lt);

            // 构造新的文件名
            std::stringstream filename;
            filename << _basename;
            filename << lt.tm_year + 1900; // 年份需要加上1900
            filename << (lt.tm_mon + 1);   // 月份从0开始，需要加1
            filename << lt.tm_mday;        // 日
            filename << lt.tm_hour;        // 小时
            filename << lt.tm_min;         // 分钟
            filename << lt.tm_sec;         // 秒
            filename << "-";
            filename << _name_count++;
            filename << ".log"; // 文件扩展名
            return filename.str();
        }

    private:
        std::string _basename; // 基础文件名，不包含扩展名和时间戳
        std::ofstream _ofs;    // 文件输出流对象，用于写入日志数据
        size_t _max_fsize;     // 单个文件的最大大小，超过该大小就会切换到新文件
        size_t _cur_fsize;     // 当前文件已写入的大小
        int _name_count;
    };

    // 工厂模式实现创建日志落地与表示的分离
    class SINK_FACTORY
    {
    public:
        template <typename SinkType, typename... Args>
        static LOG_SINK::ptr create(Args &&...args)
        {
            // 创建了一个 SinkType 类型的对象，并返回一个 std::shared_ptr
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
}

#endif