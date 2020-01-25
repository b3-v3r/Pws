#ifndef PWS_REP_H 
#define PWS_REP_H

#include "core.hpp"
#include <csignal>


namespace PwsReporter{
     void Wait();
     void Report( int num_signal );
     void WriteToJson();

};

#endif
