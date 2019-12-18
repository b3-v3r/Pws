#include "lib/main.hpp"
#include "lib/core.hpp"



int main( int argc, char **argv )
{
     PwsCore::AllStat *all_stat = new PwsCore::AllStat;

     /*
      * Set private var
      * all_stat->SetRootDir("~/")
      */

     all_stat->Init();
     
     std::this_thread::sleep_for( std::chrono::seconds(59999) );
     return 0;
}
