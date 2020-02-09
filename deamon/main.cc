#include "core.hpp"
#include "pws_reporter.hpp"
#include <sstream>

void RunServer()
{

}

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
     
     if( json_o["projects"].is_null() || json_o["host"].is_null() ||
               json_o["port"].is_null() )
     {
          std::cout << "Error reading config file, fix " << filename << " file\n";
          exit(-1);
     }

     return json_o; 
}


int main( int argc, char **argv )
{
     PwsReporter::InitReporterParams();
     if( argc != 2 )
     {
         std::cout << "Congrats !! It you first runing Pws :)\n";
         std::cout << "Make  config file ? ( y/n ): ";
          
          char choice = getchar();

          if( choice == 'y' || choice == 'Y' )
               RunServer();

          exit(1);
     }

     signal( SIGINT, &PwsReporter::Report );
     signal( SIGTERM, &PwsReporter::SaveAll );

     nlohmann::json j = ParseConfigFile( argv[1] );
     
     for( auto path : j["projects"] )
          PwsCore::AddProject(path);

     PwsCore::HandleProjects(); // cahange 

     PwsCore::HandleProccess();   

     return 0;
}
