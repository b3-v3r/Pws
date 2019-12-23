#include "lib/main.hpp"
#include "lib/core.hpp"



int main( int argc, char **argv )
{
     PwsCore::AllStat *all_stat = new PwsCore::AllStat;
     
     all_stat->AddProject( "/home/beaver/PwsProject/" );
     all_stat->AddProject( "/home/beaver/RimPkg/" );

     //
     // all_stat->Commit(); or send hup to proccess
     //
    
     std::this_thread::sleep_for( std::chrono::seconds(59999) );
     return 0;
}

/*void get_time( std::string path, std::time_t t )
{
     struct tm *tt = localtime(&t);     

     char buf[90];
     strftime(buf, 90, "%c", tt);

     std::cout << path << " : \t" << buf << "\n";
}*/
