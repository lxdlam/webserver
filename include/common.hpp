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

#include <string>
#include <regex>
#include <thread>
#include <map> // key-value container
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp> // https support
#include "external/json.hpp"  // json support

namespace WebServer
{
typedef boost::asio::ip::tcp::socket HTTP;    // HTTP socket type
typedef boost::asio::ssl::stream<HTTP> HTTPS; // HTTPS stream, which include a nomal HTTP sockets
}

#endif // COMMON_HPP