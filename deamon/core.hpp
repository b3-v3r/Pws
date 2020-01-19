#ifndef CORE_H
#define CORE_H

#include "proc_watcher.hpp"
#include "projects_watcher.hpp"
      
#include <unistd.h>
#include <stdlib.h>

#ifdef __linux__
     #include <pwd.h>
#endif


static std::vector< ProjectsWatcher* > pw;

static ProcWatcher *proc_watcher;
static WindowProperty *window_prop;

void WriteAllStat(); 
void AddProject( std::string pj_path );

void HandleProccess();

#endif
