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

void PwsReporter::Report( int num_signal )
{
     std::cout << "Start reporting .....\n";

     nlohmann::json report_j;
     
     auto windows = proc_watcher->GetAllWindows();

     for( auto &&[key, wp] : *windows )
     {
          std::cout << "Reporrw window " << wp.name << "\n";
          nlohmann::json j = {
               {"pid", wp.pid},
               {"window_open", wp.is_open},
               {"stat_per_hours", GetStatAsJson( &wp.stat_per_hours) }
          };

          report_j[wp.name] = j;
     }

     std::cout << report_j.dump();
     
}

