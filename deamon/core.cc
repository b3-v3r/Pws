#include "core.hpp"


void PwsCore::AddProject( std::string pj_path )
{
     pw.push_back( ProjectsWatcher::CreateProjectWatcher(pj_path) );
}

void PwsCore::HandleProccess()
{
     proc_watcher = new ProcWatcher(); 
}

