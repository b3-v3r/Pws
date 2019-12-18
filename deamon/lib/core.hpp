#include "projects_watcher.hpp"

namespace PwsCore {
      
     struct AllStat
     {
          std::string root_dir = "/home/beaver/";

          ProjectsWatcher *pw;
           
          void WriteAllStat();      
          void Init();
     };

}; 
