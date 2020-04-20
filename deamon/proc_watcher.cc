/*   
 * Copyright (C) Illya Adamov
 */

#include "core.hpp"
#include "proc_watcher.hpp"

void getWindowParent(Display *display, Window &win ) 
{
     Window root, parent, *child = NULL;

     unsigned int num_child;

     if( !XQueryTree(display, win, &root, 
                    &parent, &child, &num_child) )
          return;

     if( child )
          XFree((char *)child);

     win = parent;

     return;
}

void ProcWatcher::GetFocusWindow()
{
     int i = 0;

     XGetInputFocus( this->display, &this->focus_window, &i );

     getWindowParent( this->display, this->focus_window );

     if( this->focus_window == None )
     {
          sleep(1);
          GetFocusWindow();
     }

     XSelectInput( this->display, this->focus_window, FocusChangeMask );
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

std::string ProcWatcher::GetIconWindow( std::string proc_name )
{
     if( proc_name.find("terminal") != std::string::npos ) // xfce4-terminal -> terminal
          proc_name = "terminal";
     
     std::string back_path = "../client/icons";
     for( auto &picon : std_fs::recursive_directory_iterator(back_path) )
     {
          std::string filename = picon.path().filename(); 
          
          if( filename.find(proc_name) != std::string::npos )
          {
               std::string full_path = picon.path();
               return full_path.erase(0, full_path.find("icons"));
          }
     }

     return "imgs/Undefined.jpg";
}

void ProcWatcher::GetWindowProp() 
{
     int pid = 0;

     std::string proc_name;

     Atom type;
     int format;
     unsigned long nItems;
     unsigned long bytesAfter;
     unsigned char *propPid = NULL;


     int status = -1;

     while( propPid == NULL )
     {
          Atom atom_pid  = XInternAtom( this->display, "_NET_WM_PID", True );

          status = XGetWindowProperty( this->display, this->focus_window, atom_pid, 
               0, 1024, False, XA_CARDINAL, &type, &format, 
               &nItems, &bytesAfter, &propPid );
          
          if( propPid != NULL )
               break;

          XEvent xev;
          XNextEvent(this->display, &xev);
          
          if( xev.type == FocusOut ) {
               this->GetFocusWindow();
          } 
     }

     pid = *reinterpret_cast<pid_t*>(propPid);
     proc_name = this->GetProcName(pid);
     
     if( this->windows_info.find( proc_name ) == this->windows_info.end() )
     {
          WindowProperty cwp;
          cwp.w = this->focus_window;
          cwp.name = proc_name;
          cwp.pid  = pid;

          cwp.path_icon = GetIconWindow( cwp.name );

          cwp.focus_time        = std::chrono::system_clock::now();
          cwp.hour_timer        = std::chrono::system_clock::now();
          cwp.all_time          = std::chrono::milliseconds(0);
          cwp.stat_per_hours.push_back( new input_stat );
          cwp.current_hour = cwp.stat_per_hours[0];

          this->windows_info.insert( {cwp.name, cwp} );
     }
     
     this->cwindow_prop = &this->windows_info[ proc_name ];
     this->cwindow_prop->focus_time = std::chrono::system_clock::now();

     XFree(propPid);
}


void ProcWatcher::CheckExistsWindows()
{
     const auto now = std::chrono::high_resolution_clock::now();

     for( auto &&[ key, window ] : this->windows_info ) {

          auto diff_time = std::chrono::duration_cast<std::chrono::seconds>
               (now - window.hour_timer);
          
          if( diff_time.count() > 3600 )
          {
               
               window.stat_per_hours.push_back( new input_stat );
               window.current_hour = window.stat_per_hours
                    [ window.stat_per_hours.size()-1 ];

               window.hour_timer = std::chrono::system_clock::now();
               window.current_hour->all_interval = 0;
               window.current_hour->num_pressed_keys = 0;

          }
     }
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

void ProcWatcher::Start()
{
     this->display = XOpenDisplay( NULL ); // make free

     if( this->display == NULL  )
     {
          std::cout << "Display not found\n";
          exit(0);
     }

     char current_keys[32] = { 0 }, last_keys[32] = {0};

     this->GetFocusWindow(); 
     this->GetWindowProp();

     Window last_window = this->cwindow_prop->w;
     
     while( 1 ) 
     {
          usleep( 30000 );
          XQueryKeymap( this->display, current_keys );

          for( int i =0; i < 32; ++i )
          {
               if( current_keys[i] != last_keys[i] )
               {
                    if( (int)current_keys[i] != 0 )
                    {
                         int keycode = i * 8 + (int)log2( current_keys[i] ^ last_keys[i] );
                         int keysum = XKeycodeToKeysym(this->display, keycode, 0);

                         if( !IgnoreKeyNum(keysum) )
                              InputWatcher::AddInterval( this->cwindow_prop->current_hour );
                    }
               }

               last_keys[i] = current_keys[i];
          }

          this->GetFocusWindow(); 


          if( this->focus_window != last_window )
          {

               last_window = this->focus_window;

               auto now = std::chrono::system_clock::now();
               this->cwindow_prop->all_time += std::chrono::duration_cast
                    <std::chrono::milliseconds>
                    (now - this->cwindow_prop->focus_time);

               this->GetWindowProp();
          }
     }
}

Display *ProcWatcher::display = NULL;
std::map< std::string, WindowProperty > ProcWatcher::windows_info = {  };

WindowProperty *ProcWatcher::cwindow_prop = NULL;
Window ProcWatcher::focus_window = 0;

std::map< std::string, WindowProperty> *ProcWatcher::GetAllWindows()
{
     return &this->windows_info;
}

ProcWatcher::ProcWatcher()
{
     std::thread thr_timer( [this]{ 
               while( 1 )
               {
                    std::this_thread::sleep_for( std::chrono::seconds(5) );
                    this->CheckExistsWindows();
               }
     } );
     thr_timer.detach();

     Start();
}

