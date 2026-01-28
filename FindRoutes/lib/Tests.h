#ifndef RASP_API_HPP
#define RASP_API_HPP
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <chrono>
#include <filesystem>
using json = nlohmann::json;

extern int transfers;
extern const std::string API_KEY;
extern const std::string STATION_CACHE;
extern const std::string WAYS_CACHE;
extern std::string FROM_CITY;
extern std::string TO_CITY;
extern std::map<std::string, std::string> station_cache;
extern std::map<std::string, json> ways_cache;

bool is_cache_valid(const std::string& filename);

void load_station_cache();
void load_ways_cache();
void save_station_cache();
void save_ways_cache();
json get_routes(const std::string& from_code, 
               const std::string& to_code, 
               const std::string& date);
void print_routes(const json& routes);

#endif 