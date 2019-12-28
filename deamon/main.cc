#include "lib/main.hpp"
#include "lib/core.hpp"

int main( int argc, char **argv )
{
     if( getuid() != 0 )
     {
          // add reopen programm 
          std::cout << "Error: run deamon us root\n";
          exit(0);
     }

     PwsCore::AllStat *all_stat = new PwsCore::AllStat;
     
     all_stat->AddProject( "/home/beaver/PwsProject/" );
     all_stat->AddProject( "/home/beaver/RimPkg/" );
     
     all_stat->HandlePressKey();

     //
     // all_stat->Commit(); or send hup to proccess
     //
    
     std::this_thread::sleep_for( std::chrono::seconds(59999) );
     return 0;
}
