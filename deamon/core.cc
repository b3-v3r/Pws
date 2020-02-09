#include "core.hpp"


void PwsCore::AddProject( std::string pj_path )
{
     ProjectsWatcher::AddProject( pj_path );
}

void PwsCore::HandleProjects()
{
     ProjectsWatcher::Run();
}

void PwsCore::HandleProccess()
{
     proc_watcher = new ProcWatcher(); 
}

