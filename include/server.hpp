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
  explicit BaseServer(unsigned short port = 8080) : endpoint(boost::asio::ip::tcp::v4(), port),
                                                    acceptor(io_service, endpoint) {}
  BaseServer(const BaseServer &) = delete; // avoiding copy
  BaseServer(BaseServer &&) = delete;      // avoiding copy
  ~BaseServer() = default;                 // using default deconstructor

  /*!
   * @brief: 启动服务器
   * @params: 无
   * @return: 无
   * @authority: public
   * @author: Ramen
   * @since: 0.1
   */
  void start();

  resource resources;

protected:
  // Boost.asio requested for io operation
  boost::asio::io_service io_service;
  boost::asio::tcp::endpoint endpoint;
  boost::asio::tcp::acceptor acceptor;

  // all resources
  std::vector<resource::iterator> all_resources;

  /*!
   * @brief: 接受请求，纯虚函数，逻辑由各自实现
   * @params: 无
   * @return: 无
   * @authority: protected 
   * @author: Ramen
   * @since: 0.1
   */
  virtual void accept() = 0;

  /*!
   * @brief: 处理请求然后应答
   * @params: socket的共享指针s
   * @return: 无
   * @authority: protected 
   * @author: Ramen
   * @since: 0.1
   */
  void process(std::shared_ptr<socket> s);

private:
  /*!
   * @brief: 解析请求包
   * @params: 输入流，此处定向到网络io流
   * @return: 一个Request结构体，参看common.hpp
   * @authority: private
   * @author: Ramen
   * @since: 0.1
   */
  Request parseReq(std::istream &) const;
};

template <typename socket>
class Server : public BaseServer<socket>
{
};
}

void WebServer::BaseServer::start()
{
  for (auto it = resource.begin())
}

WebServer::Request WebServer::BaseServer::parseReq(std::istream &s) const
{
  Request request;
  // method, path, http version
  std::regex matcher("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
  std::smatch subMatch;

  std::string line;
  getline(s, line);
  line.pop_back();
  if (std::regex_match(line, subMatch, matcher))
  {
    request.method = subMatch[1];
    request.path = subMatch[2];
    request.version = subMatch[3];

    bool matched;
    matcher = "^([^:]*): ?(.*)$"; //pattern: "string: string"
    do
    {
      getline(s, line);
      line.pop_back();
      if (matched = std::regex_match(line, subMatch, matcher))
      {
        request.header[subMatch[1]] = subMatch[2];
      }
    } while (matched);
  }
  return request;
};

#endif // SERVER_HPP