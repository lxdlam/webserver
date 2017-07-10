/*!
 * Copyright (C), Ramen
 * File name: resources.hpp
 * Author: Ramen(lxdlam)
 * Version: 0.1
 * Date: 2017/7/11
 * Description: 路由表实现
 */

#include "common.hpp"
#include "server.hpp"

namespace WebServer
{
template <typename server>
void routerApply(server &s)
{
    s.defResources["^/?(.*)$"]["GET"] = [](std::ostream &os, Request &r) {
        std::string filename = "web/";

        std::string path = r.matcher[1];

        // 防止使用 `..` 来访问 web/ 目录外的内容
        size_t last_pos = path.rfind(".");
        size_t current_pos = 0;
        size_t pos;
        while ((pos = path.find('.', current_pos)) != std::string::npos && pos != last_pos)
        {
            current_pos = pos;
            path.erase(pos, 1);
            last_pos--;
        }

        filename += path;
        std::ifstream ifs;
        // 简单的平台无关的文件或目录检查
        if (filename.find('.') == std::string::npos)
        {
            if (filename[filename.length() - 1] != '/')
                filename += '/';
            filename += "index.html";
        }
        ifs.open(filename, std::ifstream::in);

        if (ifs)
        {
            ifs.seekg(0, std::ios::end);
            size_t length = ifs.tellg();

            ifs.seekg(0, std::ios::beg);

            // 文件内容拷贝到 response-stream 中，不应该用于大型文件
            os << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n"
               << ifs.rdbuf();

            ifs.close();
        }
        else
        {
            // 文件不存在时，返回无法打开文件
            std::string content = "Could not open file " + filename;
            os << "HTTP/1.1 400 Bad r\r\nContent-Length: " << content.length() << "\r\n\r\n"
               << content;
        }
    };
}
}