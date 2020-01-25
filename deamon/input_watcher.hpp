#ifndef INPUT_WATHER_H
#define INPUT_WATHER_H

#ifdef __linux__
     #include <linux/input.h>
     #include <sys/stat.h>
     #include <unistd.h>
     #include <fcntl.h>
     
#endif

#ifdef __linux__
     #include <X11/Xlib.h>
     #include <X11/Xutil.h>
     #include <X11/Xatom.h>
     #include <X11/XKBlib.h>

#endif


#include <thread>
#include <string>

#include <map>
#include <vector>

#include <chrono>


typedef std::chrono::high_resolution_clock::time_point ChronoTimePoint;
typedef std::chrono::milliseconds chrono_millisec;

struct input_stat
{
     int32_t all_interval     = 0;
     int32_t num_pressed_keys = 0;     

     float CanculateCPS(); // chars per second
     float CanculateCPM(); // chars per minute
};

class InputWatcher{

public:
     static void HandlerKeyPress( Display *display, 
               input_stat *current_hour,
               bool &is_window_changed );

     static void AddInterval( input_stat *current_hour );
};

#endif
