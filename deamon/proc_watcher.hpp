/*   
 * Copyright (C) Illya Adamov
 */

#ifndef PROC_WATCHER_H
#define PROC_WATCHER_H

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include <fstream>

#include <vector>
#include <map>

#ifdef __linux__
     #include <X11/Xlib.h>
     #include <X11/Xutil.h>
     #include <X11/Xatom.h>
#endif

#include "input_watcher.hpp"

/*
 * Make checking reusing window
 */

struct WindowProperty
{
     std::vector< struct input_stat *> stat_per_hours;
     struct input_stat *current_hour;

     Window w;
     bool is_open;
     std::string name;
     pid_t pid;

     std::chrono::system_clock::time_point focus_time;
      
     std::chrono::milliseconds all_time;
     ChronoTimePoint hour_timer;
     // and usage current time 
};

class ProcWatcher 
{
private:
     std::thread proc_w_thr; 

     static Display *display;

     static Window focus_window;
     static WindowProperty *cwindow_prop;

     static std::map< std::string, WindowProperty >windows_info;


     std::string GetProcName( int pid );
     void GetFocusWindow();
     void GetWindowProp();
     void CheckExistsWindows();

     void Start();
public:
     std::map< std::string, WindowProperty> *GetAllWindows();

     ProcWatcher();
     ~ProcWatcher();
};

#endif
