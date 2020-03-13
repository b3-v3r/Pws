/*   
 * Copyright (C) Illya Adamov
 */

#ifndef PWS_REP_H 
#define PWS_REP_H

#include "core.hpp"
#include <csignal>


namespace PwsReporter{
     static std::time_t time_start_deamon;

     void Report( int num_signal );
     void SaveAll( int num_signal );

     void InitReporterParams();
};

#endif
