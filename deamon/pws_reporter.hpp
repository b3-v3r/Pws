/*   
 * Copyright (C) Illya Adamov
 */

#ifndef PWS_REP_H 
#define PWS_REP_H

#include "core.hpp"
#include <csignal>


namespace PwsReporter{
     static std::time_t time_start_deamon;

     void ReadCurrentStats( nlohmann::json &report_j );

     void SaveAllStats( int num_signal );

     void InitReporterParams();

};
#endif
