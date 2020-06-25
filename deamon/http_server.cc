/*   
 * Copyright (C) Illya Adamov
 */

#include "http_server.hpp"
#include "pws_reporter.hpp"
#include "statistic.hpp"


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


// Most usage application ( time )
// Most usage application ( keys press )
// Diagrams ( js )
// Most count changes in project

void ServerHttp::HandlerStatistic( const httplib::Request &req,
         httplib::Response &res )
{
     std::ifstream json_file("../stats/history.json");

     if( !json_file.is_open() ) {
          // Return error page 
          std::cout << "Error\n";
          return;
     } 

     std::stringstream ss;
     ss << json_file.rdbuf();
      

     std::string json_str = ss.str();
     json_str[ json_str.size() -1] = ' '; // Remove  ',' on end string
     
     std::string correct_json = "{ \"all_history\" : [" + json_str + "]}";

     nlohmann::json j = nlohmann::json::parse( correct_json ); 

     std::string html_page = GetStatisticPageHtml( j );
     res.set_content( html_page, "text/html" );

     json_file.close();
}

void ServerHttp::Listener()
{
     ServerHttp::serv.set_mount_point("/", "../client/");

     ServerHttp::serv.Get( "/get_json", ServerHttp::ReturnCurrentStats );
     ServerHttp::serv.Get( "/stats", ServerHttp::HandlerStatistic );

     ServerHttp::serv.listen("127.0.0.1", 2020);
}

httplib::Server ServerHttp::serv;
