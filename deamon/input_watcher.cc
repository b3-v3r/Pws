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


bool IgnoreKeyNum( unsigned int key )
{
     static const unsigned int ignore_list_keys[] = { 
          0xff08, 0xffff, // backspace delete 
          0xff51, 0xff52, 0xff53, 0xff54, // down, up, left rigth
          0xffe9, 0xffea, // ALT_L    ALT_R 

          0xffe1, 0xffe2, // SHIFT_L  SHIFT_R
          0xffe3, 0xffe4, // CTRL_L   CTRL_R 

          // F1...F12
          0xffbe, 0xffbf, 0xffc0,0xffc1,
          0xffc2, 0xffc3, 0xffc4, 0xff5,
          0xffc7, 0xffc8, 0xffc9
     };
     for( unsigned int ikey : ignore_list_keys )
          if( key == ikey )
               return true;

     return false;
}


void InputWatcher::HandlerKeyPress( Display *display, 
          input_stat *current_hour,
          bool &is_window_changed )
{
     XEvent xev;
     XNextEvent( display, &xev );

     if( xev.type == KeyPress )
     {
          unsigned int keycode = XkbKeycodeToKeysym( display, xev.xkey.keycode, 0,
                    xev.xkey.state & ShiftMask ? 1 : 0 );

          if( !IgnoreKeyNum( keycode ) )
               InputWatcher::AddInterval( current_hour );

     } else if( xev.type == FocusOut ) {
          is_window_changed = true;
     } 

}

