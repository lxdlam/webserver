/*!
 * Copyright (C), Ramen
 * File name: http.hpp
 * Author: Ramen(lxdlam)
 * Version: 0.1
 * Date: 2017/7/9
 * Description: HTTP服务器头文件
 */

#ifndef HTTP_HPP
#define HTTP_HPP

#include "common.hpp"
#include "server.hpp"

namespace WebServer
{
// template specialization
template <>
class Server<HTTP> : public BaseServer<HTTP>
{
  public:
    Server(unsigned short port = 8080) : BaseServer<HTTP>(port){};

  private:
    /*!
   * @brief: HTTP类服务器的接受请求方法
   * @params: 无
   * @return: 无
   * @authority: private 
   * @author: Ramen
   * @since: 0.1
   */
    void accept()
    {
        auto socket = std::make_shared<HTTP>(io_service);

        acceptor.async_accept(*socket,
                              [this, socket](const boost::system::error_code &ec) {
                                  accept();
                                  if (!ec)
                                  {
                                      process(socket);
                                  }
                              });
    }
};
}

#endif // HTTP_HPP