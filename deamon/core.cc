#include "core.hpp"


void AddProject( std::string pj_path )
{
     pw.push_back( ProjectsWatcher::CreateProjectWatcher(pj_path) );
}

void HandleProccess()
{
     proc_watcher = new ProcWatcher(); 
}

