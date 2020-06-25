#ifndef STATS_H
#define STATS_H


#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>

struct app_info
{
     std::string path_icon = "/img/undefined.jpg";

     unsigned long all_press_keys  = 0;
     long using_time_ms            = 0;
};


std::string GetStatisticPageHtml( nlohmann::json json_stats );

#endif
