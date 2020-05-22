/*   
 * Copyright (C) Illya Adamov
 */

#ifndef SERV_HPP
#define SERV_HPP

#include <iostream>
#include <string>
#include <httplib.h>



class ServerHttp {

private:
     static httplib::Server serv;

     static void HandlerStatistic( const httplib::Request &req,
         httplib::Response &res );

     static void ReturnCurrentStats( const httplib::Request &req,
          httplib::Response &res );
     static void Listener();

public:
     static void RunServer();
};


#endif
