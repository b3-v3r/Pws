/*   
 * Copyright (C) Illya Adamov
 */

#ifndef PJ_WATHER_H
#define PJ_WATHER_H

#include <iostream>
#include <fstream>

#include <filesystem>
#include <thread>

#include <string>

#include <list>
#include <vector>
#include <map>
#include <array>

#include <time.h>
#include <chrono>
#include <ratio>
#include <algorithm>

#include <filesystem> 
namespace std_fs = std::filesystem;

#ifdef __linux__
     #include <sys/stat.h>
     #include <sys/types.h>
#endif 


// name type file and count use 

const std::list< std::string > supported_types_file = 
{ ".c",     ".cc",   ".c++", ".cpp", ".C" ,
  ".h",     ".hh" ,  ".hpp",

  ".py",    ".perl", ".go", ".php",
  ".html",  ".css",  ".js",

  ".ruby",  ".rb",   ".rust", ".rs",

  ".java",  ".class",
  ".cs",

  ".swift", ".R", ".d", ".dd", ".di"
};


#define DELAY_CHECKS_SEC 1

struct FileInfo {
     std_fs::path path;
      
     std::time_t last_change_time;

     int32_t num_changes;    
};

class ProjectsWatcher { 
private:
     static std::thread watcher_thread;         
     
     static std::map< std::string, 
          std::vector< FileInfo > >projects;

     bool has_TypeSupported( const std_fs::path *cpath );
     std::time_t GetTimeChangeFile( std::string );
     
     void ViewChangedFiles();
     void AddFileToWatcher( std::string project_path, std_fs::path file_path );

     void FindNewFiles( std::string project_path );

     void Handler();

public:
     static std::map< std::string, std::vector<FileInfo>> *GetProjects();
     static void AddProject( std::string project_path );

     static void Run();
};
#endif
