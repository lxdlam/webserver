/*
 * Copyright (C), Ramen
 * File name: common.hpp
 * Author: Ramen(lxdlam)
 * Version: 0.1
 * Date: 2017/07/15
 * Description: 集中常用头文件
 */
#ifndef COMMON_HPP
#define COMMON_HPP

#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <map> // key-value container
#include <unordered_map>
#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp> // https support
#include "external/json.hpp" // json support

namespace WebServer
{
struct Request
{
    std::string method;                                  //only GET and POST are acceptable
    std::string path;                                    // target path
    std::string version;                                 // http version
    std::shared_ptr<std::istream> content;               // content
    std::unordered_map<std::string, std::string> header; // headers
    std::smatch matcher;                                 // regex matcher for path
};
typedef boost::asio::ip::tcp::socket HTTP; // HTTP socket type
//typedef boost::asio::ssl::stream<HTTP> HTTPS; // HTTPS stream, which include a nomal HTTP sockets
typedef std::map<
    /* path */
    std::string, std::unordered_map<
                     /* method, lambda expression container */
                     std::string, std::function<void(std::ostream &, Request &)>>>
    resource;
}

#endif // COMMON_HPP