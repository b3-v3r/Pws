/*   
 * Copyright (C) Illya Adamov
 */

#include "input_watcher.hpp"
#include <iostream>


void InputWatcher::AddInterval( input_stat *current_hour )
{
     static ChronoTimePoint last_interval = std::chrono::high_resolution_clock::now();

     ChronoTimePoint current_time = std::chrono::high_resolution_clock::now();

     int diff_interval = std::chrono::duration_cast<chrono_millisec>
          (current_time - last_interval).count();

     if( diff_interval > chrono_millisec(1400).count() )
          diff_interval = 1400; // this first press keys of long time 

     current_hour->all_interval += diff_interval + 1;
     current_hour->num_pressed_keys++;

     last_interval = current_time;
}


float input_stat::CanculateCPM( long all_time_ms )
{
     if( num_pressed_keys == 0  || all_time_ms < 60000 )
          return num_pressed_keys;
     return this->num_pressed_keys / (all_time_ms / 1000 / 60); 
}


float input_stat::CanculateCPS()
{
     float mean_interv = (float)(this->all_interval) / 
          (float)(this->num_pressed_keys);
     
     return 1000 / mean_interv;
}

