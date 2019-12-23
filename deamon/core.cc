#include "lib/core.hpp"


namespace PwsCore {


void AllStat::AddProject( std::string pj_path )
{
     this->pw.push_back( ProjectsWatcher::CreateProjectWatcher(pj_path) );
}

}
