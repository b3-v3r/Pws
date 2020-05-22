/*   
 * Copyright (C) Illya Adamov
 */

#include "http_server.hpp"
#include "pws_reporter.hpp"


void ServerHttp::RunServer()
{
     std::thread thr( ServerHttp::Listener );
     thr.detach();
}


void ServerHttp::ReturnCurrentStats( const httplib::Request &req,
          httplib::Response &res )
{
     nlohmann::json json_raw;
     PwsReporter::ReadCurrentStats( json_raw );

     res.set_content( json_raw.dump(), "application/json" );
}

void ServerHttp::Listener()
{
     ServerHttp::serv.set_mount_point("/", "../client/");
     ServerHttp::serv.Get( "/get_json", ServerHttp::ReturnCurrentStats );
      
     ServerHttp::serv.listen("127.0.0.1", 2020);
}

void ServerHttp::HandlerStatistic( const httplib::Request &req,
         httplib::Response &res )
{
     nlohmann::json json_raw;
     // Most usage application ( time )
     // Most usage application ( keys press )
     // Diagrams ( js )
     // Most count changes in project
}

httplib::Server ServerHttp::serv;
