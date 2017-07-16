/*!
 * Copyright (C), Ramen
 * File name: main.cpp
 * Author: Ramen(lxdlam)
 * Version: 0.1
 * Date: 2017/07/06
 * Description: 主程序
 */
#include "../include/common.hpp"
#include "../include/http.hpp"
#include "../include/resources.hpp"

using namespace WebServer;

int main()
{
    Server<HTTP> s(12345);
    applyRouter<Server<HTTP>>(s);
    s.start();
    return 0;
}