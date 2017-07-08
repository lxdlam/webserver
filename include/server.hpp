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
  explicit BaseServer(unsigned short port) : endpoint(boost::asio::ip::tcp::v4(), port),
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
  resource defResources;

protected:
  // Boost.asio requested for io operation
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;

  // all resources
  std::vector<resource::iterator> allResources;

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
   * @params: socket的共享指针，用于数据传输
   * @return: 无
   * @authority: protected 
   * @author: Ramen
   * @since: 0.1
   */
  void process(std::shared_ptr<socket>) const;

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

  /*!
   * @brief: 响应请求
   * @params: socket类型的共享指针，用于数据传输；Requset类型共享指针，用于查看请求并作出相应
   * @return: 无
   * @authority: private
   * @author: Ramen
   * @since: 0.1
   */
  void respond(std::shared_ptr<socket>, std::shared_ptr<Request>) const;
};

template <typename socket>
class Server : public BaseServer<socket>
{
};
}

template <typename socket>
void WebServer::BaseServer<socket>::start()
{
  for (auto it = resources.begin(); it != resources.end(); it++)
  {
    allResources.push_back(it);
  }

  for (auto it = defResources.begin(); it != defResources.end(); it++)
  {
    allResources.push_back(it);
  }

  accept();
}

template <typename socket>
void WebServer::BaseServer<socket>::process(std::shared_ptr<socket> s) const
{
  auto readBuffer = std::make_shared<boost::asio::streambuf>();

  boost::asio::async_read_until(*s,
                                *readBuffer,
                                "\r\n\r\n" /* in http protocol, a line is end with \r\n */,
                                [this, s, readBuffer](const boost::system::error_code &ec, size_t bytes_transferred) {
                                  if (!ec)
                                  {
                                    size_t total = readBuffer->size();
                                    std::istream in(readBuffer.get());

                                    auto request = std::make_shared<Request>();
                                    *request = parseReq(in);

                                    size_t extraBytes = total - bytes_transferred;

                                    // if there is other content, read it and then
                                    if (request->header.count("Content-Length") > 0)
                                    {
                                      boost::asio::async_read(*s,
                                                              *readBuffer,
                                                              boost::asio::transfer_exactly(stoull(request->header["Content-Length"]) - extraBytes),
                                                              [this, s, readBuffer, request](const boost::system::error_code &ec, size_t bytes_transferred) {
                                                                if (!ec)
                                                                {
                                                                  request->content = std::shared_ptr<std::istream>(new std::istream(readBuffer.get()));
                                                                  respond(s, request);
                                                                }
                                                              });
                                    }
                                    else
                                    {
                                      respond(s, request);
                                    }
                                  }
                                });
}

template <typename socket>
WebServer::Request WebServer::BaseServer<socket>::parseReq(std::istream &s) const
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

template <typename socket>
void WebServer::BaseServer<socket>::respond(std::shared_ptr<socket> s, std::shared_ptr<Request> r) const
{
  for (auto res : allResources)
  {
    std::regex matcher(res->first);
    std::smatch smRes;

    // match if the path is requested
    if (std::regex_match(r->path, smRes, matcher))
    {
      // if more than 1 method is used
      if (res->second.count(r->method) > 0)
      {
        // rvalue reference
        r->matcher = std::move(smRes);
        auto writeBuffer = std::make_shared<boost::asio::streambuf>();
        std::ostream response(writeBuffer.get());
        res->second[r->method](response, *r);

        boost::asio::async_write(*s,
                                 *writeBuffer,
                                 [this, s, r, writeBuffer](const boost::system::error_code &ec, size_t bytes_transferred) {
                                   // keep alive
                                   if (!ec && stof(r->version) > 1.05)
                                     process(s);
                                 });
        return;
      }
    }
  }
}

#endif // SERVER_HPP