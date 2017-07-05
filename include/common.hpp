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

#include <regex>
#include <thread>
#include <boost/asio.hpp>
#include "external/json.hpp" // Add json support

namespace webserver{
    typedef boost::asio::ip::tcp::socket HTTP;
}

#endif // COMMON_HPP