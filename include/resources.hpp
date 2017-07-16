/*!
 * Copyright (C), Ramen
 * File name: resources.hpp
 * Author: Ramen(lxdlam) & Mikazuki
 * Version: 0.1
 * Date: 2017/7/11
 * Description: 路由表实现
 */

#include "common.hpp"
#include "server.hpp"

namespace WebServer
{

const std::string H200 = "HTTP/1.1 200 OK\r\nContent-Length: ";
const std::string H400 = "HTTP/1.1 400 Bad r\r\nContent-Length: ";

using std::cout;
using std::endl;

using json = nlohmann::json;

auto logs = [](int status, Request &r) {

    cout << r.method << " " << r.path << " " << status << endl;
};

std::unordered_map<std::string, std::string> parseParam(const std::string &p)
{
    std::unordered_map<std::string, std::string> m;
    std::regex mat("^(.*)=(.*)$");
    std::smatch res;
    std::string temp;
    int start = 0;
    do
    {
        std::regex_match(temp = p.substr(start, p.find('&', start)), res, mat);
        m[res[1]] = res[2];
    } while ((start = p.find('&', start) + 1) != std::string::npos + 1);
    return m;
}

template <typename server>
void applyRouter(server &s)
{
    //works
    s.resources["^/post/?$"]["POST"] = [](std::ostream &os, Request &r) {
        std::stringstream ss;
        *r.content >> ss.rdbuf();
        ss.seekp(0, std::ios::end);
        logs(200, r);
        os << H200 << ss.tellp() << "\r\n\r\n"
           << ss.str();
    };

    //works
    s.resources["^/json/?$"]["POST"] = [](std::ostream &os, Request &r) {
        if (r.header.count("Content-Type") > 0 &&
            r.header["Content-Type"] == "application/json")
        {
            json j;
            std::stringstream ss;
            *r.content >> j;
            ss << "<table border=\"1\"><tr><th>Key</th><th>Value</th></tr>";
            for (auto it = j.begin(); it != j.end(); it++)
            {
                ss << "<tr><td>" << it.key() << "</td><td> " << it.value() << "</td></tr>";
            }
            ss << "</table>";
            std::string content = "<h1>An request with json content</h1>" + ss.str();
            logs(200, r);
            os << H200 << content.length() << "\r\n\r\n"
               << content;
        }
        else
        {
            std::string content = "This is not a valid json content.";
            logs(400, r);
            os << H400 << content.length() << "\r\n\r\n"
               << content;
        }
    };

    s.resources["^/param/\\?(.*)$"]["GET"] = [](std::ostream &os, Request &r) {
        std::string path = r.path;
        std::unordered_map<std::string, std::string> m = parseParam(path.substr(8));
        std::stringstream ss;
        ss << "<table border=\"1\"><tr><th>Key</th><th>Value</th></tr>";
        for (auto it = m.begin(); it != m.end(); it++)
        {
            ss << "<tr><td>" << it->first << "</td><td> " << it->second << "</td></tr>";
        }
        ss << "</table>";
        std::string content = "<h1>An request with parameters</h1>" + ss.str();
        logs(200, r);
        os << H200 << content.length() << "\r\n\r\n"
           << content;
    };

    //works
    s.resources["^/infos/?$"]["GET"] = [](std::ostream &os, Request &r) {
        std::stringstream ss;
        ss << "<h1>The information about this request</h1><table border=\"1\">";
        ss << "<tr><td>Method</td><td>" << r.method << "</td></tr>";
        ss << "<tr><td>Path</td><td>" << r.path << "</td></tr>";
        ss << "<tr><td>HTTP Version</td><td>" << r.version << "</td></tr>";
        for (const auto &header : r.header)
        {
            ss << "<tr><td>" << header.first << "</td><td>" << header.second << "</td></tr>";
        }
        ss << "</table>";
        ss.seekp(0, std::ios::end);
        logs(200, r);
        os << H200 << ss.tellp() << "\r\n\r\n"
           << ss.str();
    };

    //works
    s.defResources["^/?(.*)$"]["GET"] = [](std::ostream &os, Request &r) {
        std::string filename = "web/";

        std::string path = r.matcher[1];

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

            logs(200, r);
            os << H200 << length << "\r\n\r\n"
               << ifs.rdbuf();

            ifs.close();
        }
        else
        {
            logs(400, r);
            std::string content = "Could not open file " + filename;
            os << H400 << content.length() << "\r\n\r\n"
               << content;
        }
    };

    //works
    s.defResources["^/?(.*)$"]["POST"] = [](std::ostream &os, Request &r) {
        std::string content = "This method is not acceptable.";
        logs(400, r);
        os << H400 << content.length() << "\r\n\r\n"
           << content;

    };
}
}