#pragma once
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <string>

/* 提供项目所需的基础功能接口 */
namespace jl12800
{
    namespace utill
    {
        /* 获取系统时间*/
        class date
        {
        public:
            static time_t now()
            {
                return time(nullptr);
            }
        };

        /* 文件相关操作接口-判断文件存在与否、文件路径、创建目录 */
        class file
        {
        public:
            static bool exists(const std::string &filename)
            {
                struct stat file_buf;
                return stat(filename.c_str(), &file_buf) == 0;
            }

            static std::string filepath(const std::string &filepath)
            {
                // 文件路径为空
                if (filepath.empty())
                    return ".";

                // 文件路径不为空，查找文件路径分隔符
                size_t pos = filepath.find_last_of("/\\");
                // 没有查到分隔符，表示文件在当前目录
                if (pos == std::string::npos)
                    return ".";
                // 查到文件分隔符，截取子串返回文件路径
                return filepath.substr(0, pos + 1);
            }

            static void creat_directory(const std::string &path)
            {
                size_t pos = 0, idx = 0;

                // 循环处理路径中的每个部分，逐级创建目录
                while (idx < path.size())
                {
                    // 查找下一个斜杠的位置，斜杠分隔不同级别的目录
                    pos = path.find_first_of("/\\", idx);

                    // 没有找到，说明是最后一级目录
                    if (pos == std::string::npos)
                    {
                        mkdir(path.c_str(), 0777);
                        return;
                    }

                    // 找到了，获取当前级别的父目录
                    std::string parent_dir = path.substr(0, pos + 1);
                    idx = pos + 1;

                    // 如果父目录存在，跳过本次循环
                    if (exists(parent_dir.c_str()) == true)
                        continue;
                    // 如果父目录不存在，创建
                    mkdir(parent_dir.c_str(), 0777);
                }
            }
        };
    }
}