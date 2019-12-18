#include "lib/core.hpp"


namespace PwsCore {


void AllStat::Init()
{
     this->pw = new ProjectsWatcher( this->root_dir );
}

}
