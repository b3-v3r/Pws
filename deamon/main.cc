#include "core.hpp"

int main( int argc, char **argv )
{
     AddProject( "/home/beaver/PwsProject/" );
     AddProject( "/home/beaver/RimPkg/" );
     
     HandleProccess();   

     //
     // all_stat->Commit(); or send hup to proccess
     //
    
     std::this_thread::sleep_for( std::chrono::seconds(59999) );
     return 0;
}
