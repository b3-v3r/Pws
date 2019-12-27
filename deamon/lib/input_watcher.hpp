#ifndef INPUT_WATHER_H
#define INPUT_WATHER_H

#ifdef __linux__
     #include <linux/input.h>
     #include <sys/stat.h>
     #include <unistd.h>
     #include <fcntl.h>
#endif

#include <thread>
#include <string>
#include <vector>
#include <chrono>

typedef std::chrono::high_resolution_clock ChronoClock;
typedef std::chrono::milliseconds chrono_millisec;

class InputWather
{
public:
     struct input_stat;

     InputWather();
     ~InputWather();

private:
     std::thread input_thread;

     std::vector< input_stat *> stat_per_hours;

     struct input_stat *current_stat;     

     void EventTimer();
     void AddInterval( ChronoClock::time_point &last_interval );

     void HandlerKeyPress();

};

struct InputWather::input_stat
{
     int32_t all_interval      = 0;
     int32_t num_pressed_keys = 0;     

     float CanculateCPS(); // chars per second
     float CanculateCPM(); // chars per minute
};
#endif
