#include "statistic.hpp"

bool SortAppsByUsingTime( const std::pair<std::string, app_info> &el1, 
          const std::pair<std::string, app_info> &el2 )
{
     return el1.second.using_time_ms > el2.second.using_time_ms;
}

std::string milliseconds_toStr( const long time_ms )
{
     unsigned int hours = time_ms / 3600000;
     unsigned int minutes = (time_ms - 3600000 * hours) / 60000;
     unsigned int seconds = (time_ms - 3600000 * hours - 60000 * minutes) / 1000; 

     return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
}


std::string StatsToHtml( std::vector< std::pair< std::string, app_info> > &all_apps_vec )
{
     std::string html_page = "<!DOCTYPE html> \
     <html lang=\"en\">\
     <head>\
          <meta charset=\"UTF-8\">\
          <title>Statistic</title>\
          <link rel=\"stylesheet\" href=\"stats.css\">\
     </head>\
     <body>\
          <table class=\"container\"> \
               <thead> \
                    <tr> \
                         <th><h1>Icon</h1></th> \
                         <th><h1>App Name</h1></th> \
                         <th><h1>All time using</h1></th>\
                         <th><h1>Count press keys</h1></th> \
                    </tr> \
               </thead> \
               <tbody>";

     for( auto app : all_apps_vec )
     {
          html_page += "<tr>";
          html_page += "<td><img src=\" " + app.second.path_icon + "\"> </td>";
          html_page += "<td>" + app.first + "</td>";
          html_page += "<td>" + milliseconds_toStr(app.second.using_time_ms) + " </td>";
          html_page += "<td>" + std::to_string(app.second.all_press_keys) + " </td>";

          html_page += "</tr>";
     }
     
     html_page += "</tbody></table></body></html>";
     return html_page;
}

std::string GetStatisticPageHtml( nlohmann::json json_stats )
{
     std::map< std::string, app_info > all_apps;

     for( auto &all_stat_j : json_stats["all_history"] )
     {
          for( auto &app_j : all_stat_j["windows"] )
          {
               std::string app_name = app_j["name"];
               unsigned long num_press_keys = 0;


               for( auto &stat_per_hour : app_j["stat_per_hours"] )
                    if( !stat_per_hour["num_pressed_keys"].is_null() )
                         num_press_keys += static_cast<unsigned long>(stat_per_hour["num_pressed_keys"]);

               all_apps[ app_name ].using_time_ms += static_cast<long>(app_j["all_time"]);
     
               all_apps[ app_name ].all_press_keys += num_press_keys;
               all_apps[ app_name ].path_icon = app_j["path_icon"];

          }
     }

     // Convert map to vector for sorting 
     std::vector< std::pair<std::string, app_info> > all_apps_vec;
     for( auto &[key,val] : all_apps )
          all_apps_vec.push_back( {key, val} );

     std::sort( all_apps_vec.begin(),all_apps_vec.end(),SortAppsByUsingTime );
     
     return StatsToHtml( all_apps_vec );
}
