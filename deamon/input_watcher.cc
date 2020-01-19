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

     std::cout << "Press key\n";
}


float input_stat::CanculateCPM()
{
     return CanculateCPS() * 60;
}


float input_stat::CanculateCPS()
{
     float mean_interv = (float)(this->all_interval) / 
          (float)(this->num_pressed_keys);
     
     return 1000 / mean_interv;
}


/*void InputWatcher::StartEventTimer()
{
     std::thread t( [ this ](){
               
               while( 1 )
               { // check 
                    std::this_thread::sleep_for( std::chrono::hours(1) );
                    
                    this->stat_per_hours.push_back( new struct input_stat );
                    this->current_stat = (*(--this->stat_per_hours.end()));
               }
          });
     t.detach();
}*/


void InputWatcher::HandlerKeyPress( Display *display, 
          input_stat *current_hour,
          bool &is_window_changed )
{
     XEvent xev;
     XNextEvent( display, &xev );

     if( xev.type == KeyPress )
          InputWatcher::AddInterval( current_hour );

     else if( xev.type == FocusOut )
          is_window_changed = true;
}

