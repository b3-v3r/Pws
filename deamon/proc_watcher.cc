#include "core.hpp"
#include "proc_watcher.hpp"

void ProcWatcher::GetFocusWindow()
{
     int i = 0;
     XGetInputFocus( this->display, &this->focus_window, &i );
     XSelectInput( this->display, this->focus_window, KeyPressMask | 
               FocusChangeMask);//KeyReleaseMask )
} 

std::string ProcWatcher::GetProcName( int pid )
{
     std::string proc_name;

     static int last_pid = 0;
     static std::string proc_name_res; 

     if( last_pid == pid )
          return proc_name_res;

     last_pid = pid;

     std::string pid_s = std::to_string(pid);

     std::ifstream file("/proc/"+pid_s+"/cmdline");
     std::getline( file, proc_name, (char)file.eof() );

     //
     // trim procces name      
     //
     proc_name_res = "";
     for( char c : proc_name )
     {
          if( c == ' ' )
               break;     
          else if( c == '/' )
          {
               proc_name_res = "";
               continue;
          }
          
          proc_name_res.push_back(c);
     } 

     file.close();

     return proc_name_res;
}

void ProcWatcher::GetWindowProp() 
{
     int pid = -1;
     WindowProperty cwp;

     std::string proc_name;

     Atom type;
     int format;
     unsigned long nItems;
     unsigned long bytesAfter;
     unsigned char *propPid = 0;

     Atom atom_pid  = XInternAtom( this->display, "_NET_WM_PID", True );

     int status = XGetWindowProperty( this->display, this->focus_window, atom_pid, 
               0, 1, False, XA_CARDINAL, &type, &format, 
               &nItems, &bytesAfter, &propPid );
     
     if( status == Success ) 
          pid = *reinterpret_cast<pid_t*>(propPid);

     proc_name = this->GetProcName(pid);

     if( this->windows_info.find( proc_name ) == this->windows_info.end() )
     {
          cwp.w = this->focus_window;
          cwp.name = proc_name;
          cwp.pid  = pid;

          cwp.last_usage_window = std::chrono::system_clock::now();
          cwp.start_time        = std::chrono::system_clock::now();

          cwp.stat_per_hours.push_back( new input_stat );
          cwp.current_hour = cwp.stat_per_hours[0];

          this->windows_info.insert( {cwp.name, cwp} );
     }
     
     this->cwindow_prop = &this->windows_info[ proc_name ];

     // при первом импользовании 
     // Free atoms
}

void ProcWatcher::CheckExistsWindows()
{
     const auto now = std::chrono::high_resolution_clock::now();

     for( auto [ key, window ] : this->windows_info )     
     {
          if( window.is_open )
          {
               auto diff_time = std::chrono::duration_cast<std::chrono::seconds>
                    (now - window.hour_timer);
               
               if( diff_time.count() > 3600)
               {
                    window.stat_per_hours.push_back( new input_stat );
                    window.current_hour = window.stat_per_hours
                         [ window.stat_per_hours.size() ];
               }

          }
     }
}

void ProcWatcher::Start()
{
     this->display = XOpenDisplay( 0 ); // make free

     bool is_window_changed = false;

     this->GetFocusWindow();
     this->GetWindowProp();

     while( 1 ) 
     {
          InputWatcher::HandlerKeyPress( this->display, 
                    this->cwindow_prop->current_hour, 
                    is_window_changed );          
           
          std::cout << "Window -> " << this->cwindow_prop->name << " - " 
                    << this->cwindow_prop->pid << "\n";

          if( is_window_changed )
          {
               this->GetFocusWindow();
               this->GetWindowProp();

               is_window_changed = false;
          }
     }
}

Display *ProcWatcher::display = NULL;
std::map< std::string, WindowProperty > ProcWatcher::windows_info = {  };

WindowProperty *ProcWatcher::cwindow_prop = NULL;
Window ProcWatcher::focus_window = 0;

ProcWatcher::ProcWatcher()
{
    this->proc_w_thr = std::thread( &ProcWatcher::Start, this );
    this->proc_w_thr.detach();
}

