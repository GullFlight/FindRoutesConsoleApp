#include "Tests.h"
using json = nlohmann::json;


int transfers = 1;
const int CACHE_HOURS = 24;
const std::string API_KEY = "84add5c9-8bb0-4efb-92d3-e3cb621bd83f";
const std::string STATION_CACHE =  "../bin/stations.json";
const std::string WAYS_CACHE =  "../bin/ways.json";
std::string FROM_CITY = "Санкт-Петербург";
std::string TO_CITY = "Псков"; 

std::string From_City_Code = "c2";
std::string To_City_Code = "c25";
std::map<std::string, std::string> station_cache;
std::map<std::string, json> ways_cache;

bool is_cache_valid(const std::string& filename) {
    if (!std::filesystem::exists(filename))
        return false;
    
    auto last_write = std::filesystem::last_write_time(filename);
    auto now = std::filesystem::file_time_type::clock::now();
    auto diff = now - last_write;
    
    return diff < std::chrono::hours(CACHE_HOURS);
}

void load_station_cache() {
    const std::string cache_file = "../bin/stations.json";
    if (!is_cache_valid(cache_file)) return;

    std::ifstream file(cache_file);
    if (file) station_cache = json::parse(file).get<std::map<std::string, std::string>>();
}

void load_ways_cache() {
    const std::string cache_file = "../bin/ways.json";
    if (!is_cache_valid(cache_file)) return;

    std::ifstream file(cache_file);
    if (file) ways_cache = json::parse(file).get<std::map<std::string, json>>();
}

void save_station_cache() {
    std::ofstream file("../bin/stations.json");
    if (file) file << json(station_cache).dump(2);
}

void save_ways_cache() {
    std::ofstream file("../bin/ways.json");
    if (file) file << json(ways_cache).dump(2);
}


json get_routes(const std::string& From_code, const std::string& To_code, const std::string& Date) {
    if(ways_cache.count(FROM_CITY + '-' + TO_CITY + '-' + std::to_string(transfers))){
        return ways_cache[FROM_CITY + '-' + TO_CITY + '-' + std::to_string(transfers)];
    }
    cpr::Response response = cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"}, 
        cpr::Parameters{
            {"apikey", API_KEY},
            {"from", From_code}, 
            {"to", To_code},
            {"date", Date},
            {"transfers", (transfers == 0) ? "false" :"true"}
        }
    );

    if (response.status_code != 200) {
        throw std::runtime_error("Ошибка HTTP: " + std::to_string(response.status_code));
    }
    json result = json::parse(response.text);
    ways_cache[FROM_CITY + '-' + TO_CITY + '-' + std::to_string(transfers)] = result;

    return result;

}

void print_routes(const json& routes) {
    std::cout << "Найдено маршрутов: " << routes["segments"].size() << "\n\n";
    for (const auto& segment : routes["segments"]) {
        if (segment.contains("details") && segment["details"].size() - 2 > transfers) {
            continue;
        }
        if (segment.contains("thread")) {
            if (segment["thread"].contains("transport_type")) {
                std::cout << "ТИП ТРАНСПОРТА: " << segment["thread"]["transport_type"] << "\n";
            }
            if(segment.contains("departure")){
                            std::cout << "ВРЕМЯ ОТПРАВЛЕНИЯ: " << segment["departure"] << std::endl;
                        }
                        if(segment.contains("arrival")){
                            std::cout << "ВРЕМЯ ПРИБЫТИЯ: " << segment["arrival"] << std::endl;
                        }
                        if(segment.contains("duration")){
                            std::cout << "ДЛИТЕЛЬНОСТЬ: " << segment["duration"] << " мин " 
                            << std::endl;
                        }
        }
        else if (segment.contains("details")) {
            std::cout << "МАРШРУТ С ПЕРЕСАДКАМИ:\n";
            
            std::cout << "ОТ: " << segment["departure_from"]["title"]
                     << "\nДО: " << segment["arrival_to"]["title"]
                     << "\nВРЕМЯ ОТПРАВЛЕНИЯ: " << segment["departure"]
                     << "\nВРЕМЯ ПРИБЫТИЯ: " << segment["arrival"]
                     << "\nКОЛИЧЕСТВО ПЕРЕСАДОК: " << segment["transfers"].size(); 
                     std::cout << "\n---- ПЕРЕСАДКА ----\n";       
            for (const auto& detail : segment["details"]) {
                if (detail.contains("is_transfer")) {
                    std::cout << "МЕСТО ПЕРЕСАДКИ: " << detail["transfer_point"]["title"]
                             << "\nДЛИТЕЛЬНОСТЬ: " << detail["duration"]<< " мин\n";
                } else {
                    std::cout << "ТИП ТРАНСПОРТА: " << detail["thread"]["transport_type"]
                             << "\nОТ: " << detail["from"]["title"]
                             << "\nДО: " << detail["to"]["title"]
                             << "\nВРЕМЯ ОТПРАВЛЕНИЯ: " << detail["departure"] 
                             << "\nВРЕМЯ ПРИБЫТИЯ: " << detail["arrival"] << "\n";
                }
            }
            std::cout << "===========================\n";
            continue;
        }
        if (segment.contains("tickets_info")) {
            const auto& tickets_info = segment["tickets_info"];
            if (tickets_info["places"].empty()) {
                std::cout << "ЦЕНА: Не указана\n";
            } else {
                for (const auto& place : tickets_info["places"]) {
                    std::cout << "ЦЕНА: " << place["price"]["whole"] << " RUB\n";
                }
            }
        }
        std::cout << "----------------------\n";
    }
}