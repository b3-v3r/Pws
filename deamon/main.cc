/*   
 * Copyright (C) Illya Adamov
 */

#include "core.hpp"
#include "pws_reporter.hpp"
#include <sstream>

void RunServer()
{
     system( ("go run ../client/main.go " + std::to_string(getpid()) + " &").c_str() );
}


/* TODO
 * Make reopen config in real-time
 * Make configurate project from web page 
 * Notes and tasks 
 * Change design web page 
 * Logging errors
 * Grap pixmap form window X11
 */

nlohmann::json ParseConfigFile( char *filename )
{
     std::ifstream file(filename);
     std::string config_text;

     if( file.is_open() ) {
          std::stringstream ss;

          ss << file.rdbuf();
          config_text = ss.str();
     }else{
          std::cout << "File config [" << filename << "] not exist\n";
          exit(-1);
     }
     
     nlohmann::json json_o = nlohmann::json::parse(config_text);
     
     if( json_o["projects"].is_null() )
     {
          std::cout << "Error reading config file, fix " << filename << " file\n";
          exit(-1);
     }

     return json_o; 
}

int err_handler( Display *d, XErrorEvent *ev )
{
     std::cout << "Error x11 " << ev->error_code << "\n";  
     return 0;
}

int main( int argc, char **argv )
{
     XSetErrorHandler( err_handler );
     PwsReporter::InitReporterParams();

     if( argc != 2 )
     {
          std::cout << "Congrats !! It you first runing Pws :)\n";
          std::cout << "Example runing: ./PwsCore config.json\n";
          exit(1);
     }

     pid_t pid = fork();

     if( pid == 0 )
     {
          signal( SIGINT, &PwsReporter::Report );
          signal( SIGTERM, &PwsReporter::SaveAll );

          nlohmann::json j = ParseConfigFile( argv[1] );
          

          for( auto path : j["projects"] )
               PwsCore::AddProject(path);

          RunServer();

          PwsCore::HandleProjects();  
          PwsCore::HandleProccess();   

     }else if( pid > 0 ) {
          std::cout << "Pws deamon start\n";
          exit(0);
     }

     return 0;
}
