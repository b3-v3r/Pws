#include "projects_watcher.hpp"

namespace PwsCore {
      
     struct AllStat
     {
          std::vector< ProjectsWatcher* > pw;
           
          void WriteAllStat(); 
          void AddProject( std::string pj_path );

     };

}; 
