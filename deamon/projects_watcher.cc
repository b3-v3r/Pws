#include "projects_watcher.hpp"


void get_time( std::string path, std::time_t t )
{
     struct tm *tt = localtime(&t);     

     char buf[90];
     strftime(buf, 90, "%c", tt);

     std::cout << path << " : \t" << buf << "\n";
}

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

void ProjectsWatcher::AddFileToWatcher( std_fs::path file_path )
{
     std::string file_path_str = file_path.u8string();

     auto is_equal = [file_path_str]( FileInfo fi ) {
          return fi.path.u8string() == file_path_str;
     };
     
     auto is_found = std::find_if( this->changed_files.begin(),
                                    this->changed_files.end(), is_equal);
     
     if( is_found != this->changed_files.end() )
          return;

     std::cout << "Add file " << file_path_str << "\n";
     std::time_t time_change = this->GetTimeChangeFile( file_path_str ); 

     this->changed_files.push_back( 
          {
               .path = file_path,
               .last_change_time = time_change,

               .original_time_change = time_change,
               .original_size = std_fs::file_size( file_path ),

               .num_changes = 0,
          });
}


void ProjectsWatcher::FindNewChangeFiles( std::string project_path )
{ 
     for( const auto &path_it : std_fs::recursive_directory_iterator(project_path) )
     {
          if(this->has_TypeSupported( &path_it.path() ))
               this->AddFileToWatcher( path_it.path() ); 
     }
} 

void ProjectsWatcher::ViewChangedFiles()
{
     for( auto &file : this->changed_files )
     {
          std::time_t current_change_t = this->GetTimeChangeFile(file.path.u8string());

          if( current_change_t > file.last_change_time )
          {
               file.num_changes++;
               file.last_change_time = current_change_t;
          }
     }
}

void ProjectsWatcher::Run( std::string project_path )
{
     int check_i = 0;
     this->FindNewChangeFiles( project_path );

     while( 1 ) 
     {
          if( check_i++ == 10 ) {
               this->FindNewChangeFiles( project_path );
               check_i = 0;
          }

          this->ViewChangedFiles();
          std::this_thread::sleep_for( std::chrono::seconds( DELAY_CHECKS_SEC ) );
     }
}

/*std::string *ProjectsWatcher::GetRootPath()
{

}*/

ProjectsWatcher *ProjectsWatcher::CreateProjectWatcher( std::string project_path )
{
     ProjectsWatcher *pjw = new ProjectsWatcher; 

     pjw->watcher_thread = std::thread( &ProjectsWatcher::Run,
               pjw, project_path);
     
     return pjw;
}


