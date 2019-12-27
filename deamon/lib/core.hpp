#include "projects_watcher.hpp"
#include "input_watcher.hpp"

namespace PwsCore {
      
     struct AllStat
     {
          std::vector< ProjectsWatcher* > pw;
          InputWather *iw;
           
          void WriteAllStat(); 
          void AddProject( std::string pj_path );

          void HandlePressKey();
     };

}; 
