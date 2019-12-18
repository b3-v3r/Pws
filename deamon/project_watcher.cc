#include "lib/projects_watcher.hpp"


ProjectsWatcher::ProjectsWatcher( std::string root_path)
{
     this->watcher_thread = std::thread( &ProjectsWatcher::Run,
               this, root_path);
}

std::time_t ProjectsWatcher::GetTimeChangeFile( std::string path )
{
#ifdef __linux__
          struct stat st;
          stat( path.c_str(), &st );
          return st.st_mtime;
#endif
}

enum PATH_TYPE ProjectsWatcher::GetPathType( const std_fs::path *cpath )
{
     if( std_fs::is_directory( *cpath ) )
          return TYPE_DIR;

     std::string file_type = cpath->extension();

     for( auto i : supported_types_file )
          if( file_type == i )
               return FTYPE_SUPPORTED;
     
     return FTYPE_NOT_SUPPORTED;
}                    

void ProjectsWatcher::AddFileToWatcher( std_fs::path file_path, 
                                        std::time_t time_change )
{
     std::string path_str = file_path.u8string();

     auto is_equal = [path_str]( FileInfo fi ) {
          return fi.path.u8string() == path_str;
     };
     
     auto is_found = std::find_if( this->changed_files.begin(),
                                    this->changed_files.end(), is_equal);
     
     if( is_found != this->changed_files.end() )
          return;

     this->changed_files.push_back( 
          {
               .path = file_path,
               .original_time_change = time_change,
               .original_size = std_fs::file_size( file_path ),
               .num_changes = 1
          });
}


void ProjectsWatcher::FindNewChangeFiles( std::string root_path, std::time_t &last_change_time, std::time_t &res )
{ 

     for( const auto &path_it : std_fs::directory_iterator(root_path) )
     {
          std::time_t current_change_t = this->GetTimeChangeFile( path_it.path().u8string() ); 

          if( current_change_t > last_change_time )
          {
               res = current_change_t;

               int type_path = this->GetPathType( &path_it.path() );

               if( type_path == TYPE_DIR )
                    this->FindNewChangeFiles( path_it.path(), last_change_time, res);
               else if( type_path == FTYPE_SUPPORTED )
                    this->AddFileToWatcher( path_it.path(), current_change_t );
          }
     }
} 


void ProjectsWatcher::Run( std::string root_path )
{
     std::time_t last_change_time = this->GetTimeChangeFile( root_path );
     std::time_t res = last_change_time; 

     while( 1 ) 
     {
          this->FindNewChangeFiles( root_path, last_change_time, res);
          last_change_time = res; 

          std::this_thread::sleep_for( std::chrono::seconds( DELAY_CHECKS_SEC ) );
     }
}
