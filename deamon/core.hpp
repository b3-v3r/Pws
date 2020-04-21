/* 
 * Copyright (C) Illya Adamov
 */

#ifndef CORE_H
#define CORE_H

#include "proc_watcher.hpp"
#include "projects_watcher.hpp"
      
#include <unistd.h>
#include <stdlib.h>

#ifdef __linux__
     #include <pwd.h>
     #include <nlohmann/json.hpp>
#endif


static ProcWatcher *proc_watcher;

namespace PwsCore {
     struct{
          std::vector< std::string >projects;
     }config;

     void AddProject( std::string pj_path ); 

     void HandleProjects();
     void HandleProccess();
}

#endif
