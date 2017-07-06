/*
 * Copyright (C), Ramen
 * File name: server.hpp
 * Author: Ramen(lxdlam)
 * Version: 0.1
 * Date: 2017/07/05
 * Description: 服务器类头文件实现
 */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"

namespace WebServer
{
typedef std::map<
    /* path */
    std::string, std::unordered_map<
                     /* method, lambda expression container */
                     std::string, std::function<void(std::ostream &, Request &)>>>
    resources;
/*!
  * @brief: 服务器基类，传入socket类型作为模板
  * @base: 无基类
  * @author: Ramen
  * @version: 0.1
  */
template <typename socket>
class BaseServer
{
public:
  explicit BaseServer() : port(8000), isLog(false) {} // set port to 8000, no log
  BaseServer(const BaseServer &) = delete;            // avoiding copy
  BaseServer(BaseServer &&) = delete;                 // avoiding copy
  ~BaseServer() = default;                            // using default deconstructor

private:
  unsigned int port; // the port
  bool isLog;
};
}

#endif // SERVER_HPP