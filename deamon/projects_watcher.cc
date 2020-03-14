/*   
 * Copyright (C) Illya Adamov
 */

#include "projects_watcher.hpp"


std::time_t ProjectsWatcher::GetTimeChangeFile( std::string path )
{
#ifdef __linux__
          struct stat st;
          stat( path.c_str(), &st );
          return st.st_mtime;
#endif
}

bool ProjectsWatcher::has_TypeSupported( const std_fs::path *cpath )
{
     std::string file_type = cpath->extension();

     for( auto i : supported_types_file )
          if( file_type == i )
               return true;
     
     return false;
} 

void ProjectsWatcher::AddFileToWatcher( std::string project_path, 
          std_fs::path file_path )
{
     std::string file_path_str = file_path.u8string();
 
     auto is_equal = [file_path_str]( FileInfo fi ) 
     {
          return fi.path.u8string() == file_path_str;
     };
     
     auto is_found = std::find_if( this->projects[project_path].begin(),
                                    this->projects[project_path].end(), is_equal);
     
     if( is_found != this->projects[project_path].end() )
          return;

     std::time_t time_change = this->GetTimeChangeFile( file_path_str );  
     
     this->projects[project_path].push_back( 
          {
               .path = file_path,
               .last_change_time = time_change,

               .num_changes = 0,
          });
}


void ProjectsWatcher::FindNewFiles( std::string project_path )
{ 
     for( const auto &path_it : 
               std_fs::recursive_directory_iterator(project_path) )
     {
          if( this->has_TypeSupported( &path_it.path()) )
               this->AddFileToWatcher( project_path, path_it.path()); 
     }
} 

void ProjectsWatcher::ViewChangedFiles()
{
     for( auto &[ key, projects ] : this->projects )
     {
          for( auto &file : projects )
          {
               std::time_t current_change_t = this->GetTimeChangeFile(file.path.u8string());

               if( current_change_t > file.last_change_time )
               {
                    file.num_changes++;
                    file.last_change_time = current_change_t;
               }
          }
     }
}

void ProjectsWatcher::Handler()
{
     int check_i = 0;

     for( auto &&[pj_path, files] : this->projects ) 
          this->FindNewFiles( pj_path );

     while( 1 )
     {
          for( auto &&[pj_path, files] : this->projects ) 
          {
               if( check_i++ == 10 ) {
                    this->FindNewFiles( pj_path );
                    check_i = 0;
               }

               this->ViewChangedFiles();
               std::this_thread::sleep_for( std::chrono::seconds( DELAY_CHECKS_SEC ) );
          }
     }
}

std::map< std::string, std::vector<FileInfo>> *ProjectsWatcher::GetProjects()
{
     return &ProjectsWatcher::projects;
}


std::thread ProjectsWatcher::watcher_thread;
std::map< std::string, std::vector< FileInfo > > ProjectsWatcher::projects;


void ProjectsWatcher::Run()
{
     ProjectsWatcher::watcher_thread = std::thread( &ProjectsWatcher::Handler, 
               new ProjectsWatcher() );
     ProjectsWatcher::watcher_thread.detach();
}


void ProjectsWatcher::AddProject( std::string project_path )
{
     //
     // check exist
     // check sumlinks on .. or .
     
     std::vector< FileInfo > tmp;
     ProjectsWatcher::projects[project_path] = tmp;
}

