/*   
 * Copyright (C) Illya Adamov
 */

#include "pws_reporter.hpp"

time_t GetDiffTime( time_t change_time )
{
     time_t now = time(NULL);
     time_t diff = now - change_time;
    
     return diff; 
}


nlohmann::json GetStatAsJson( std::vector< struct input_stat*> *stats )
{
     nlohmann::json j = nlohmann::json::array();

     int num_hour = 1;

     for( auto stat : *stats )
     {
          nlohmann::json stat_j;

          stat_j["num_hour"] = num_hour;
          stat_j["num_pressed_keys"] = stat->num_pressed_keys;

          stat_j["CPS"] = stat->CanculateCPS();
          stat_j["CPM"] = stat->CanculateCPM();

          j.push_back(stat_j);

          num_hour++;
     }

     return j;
}

void ReportToFile( std::string path )
{
     nlohmann::json report_j;

     nlohmann::json projects_j;
     nlohmann::json windows_j = nlohmann::json::array();

     auto windows = proc_watcher->GetAllWindows();

     for( auto &&[key, wp] : *windows )
     {
          nlohmann::json j = {
               {"name", wp.name},
               {"pid", wp.pid},
               {"window_open", wp.is_open},
               {"all_time", wp.all_time.count() },
               {"stat_per_hours", GetStatAsJson( &wp.stat_per_hours) }
          };

          windows_j.push_back(j);
     }

     auto projects = ProjectsWatcher::GetProjects(); 

     for( auto &[project_path, changed_files] : *projects )
     {
          nlohmann::json list_files = nlohmann::json::array();

          for( auto &change_file : changed_files )
          {
               if( change_file.num_changes == 0 )
                    continue;

               nlohmann::json j = {
                    {"path", change_file.path},
                    {"num_changes", change_file.num_changes},
                    {"last_change_time", GetDiffTime(change_file.last_change_time)}
               };

               list_files.push_back( j );
          }
          
          nlohmann::json project_info_j = {
               {"project_path", project_path},
               {"files", list_files}
          };

          projects_j.push_back( project_info_j );
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
     ReportToFile("../stats/current_stat.json");
}

void PwsReporter::SaveAll( int num_signal )
{
     PwsReporter::Report( 0 );
     ReportToFile("../stats/" + 
               std::to_string( PwsReporter::time_start_deamon ) + ".json");
     exit(0);     
}

void PwsReporter::InitReporterParams()
{
     PwsReporter::time_start_deamon = time(NULL);
}
