#include "pws_reporter.hpp"

nlohmann::json GetStatAsJson( std::vector< struct input_stat*> *stats )
{
     nlohmann::json j;

     int num_hour = 0;
     for( auto stat : *stats )
     {
          nlohmann::json stat_j;
          stat_j["num_pressed_keys"] = stat->num_pressed_keys;

          stat_j["CPS"] = stat->CanculateCPS();
          stat_j["CPM"] = stat->CanculateCPM();

          j[ std::to_string(num_hour) ] = stat_j;

          num_hour++;
     }

     return j;
}

void ReportToFile( std::string path )
{
     nlohmann::json report_j;

     nlohmann::json windows_j;
     nlohmann::json projects_j;
     
     auto windows = proc_watcher->GetAllWindows();

     for( auto &&[key, wp] : *windows )
     {
          nlohmann::json j = {
               {"pid", wp.pid},
               {"window_open", wp.is_open},
               {"all_time", wp.all_time.count() },
               {"stat_per_hours", GetStatAsJson( &wp.stat_per_hours) }
          };

          windows_j[wp.name] = j;
     }

     auto projects = ProjectsWatcher::GetProjects(); 

     for( auto &[project_name, changed_files] : *projects )
     {
          nlohmann::json list_files;

          for( auto &change_file : changed_files )
          {
               if( change_file.num_changes == 0 )
                    continue;
               nlohmann::json j = {
                    {"path", change_file.path},
                    {"num_changes", change_file.num_changes},
                    {"last_change_time", change_file.last_change_time}
               };
               list_files[project_name].push_back( j );
          }

          projects_j.push_back( list_files );
     }
      
     report_j["time_start_deamon"] = PwsReporter::time_start_deamon;
     report_j["time_save_report"] = time(NULL);

     report_j["windows"] = windows_j;
     report_j["projects"] = projects_j;

     std::ofstream file_result( path );
     file_result << report_j;
}

void PwsReporter::Report( int num_signal )
{
     ReportToFile("/home/beaver/pws/stats/current_stat.json");
}

void PwsReporter::SaveAll( int num_signal )
{
     ReportToFile("/home/beaver/pws/stats/" + 
               std::to_string( PwsReporter::time_start_deamon ) + ".json");
     exit(0);     
}

void PwsReporter::InitReporterParams()
{
     PwsReporter::time_start_deamon = time(NULL);
}
