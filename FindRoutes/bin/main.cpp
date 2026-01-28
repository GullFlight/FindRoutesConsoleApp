#include "../lib/Tests.h"

int main(int argc, char** argv) {
    std::ofstream file("../bin/out.txt");
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc != 2) {
        std::cerr << "Неправильный формат ввода";
        return 1;
    }

    std::vector<std::string> Args;
    Args.push_back(argv[0]);
    Args.push_back(argv[1]);

    std::string date = Args[1];    
    load_station_cache();
    json routes = get_routes("c2", "c25", date);
    file << routes;
    file.close();
    print_routes(routes);
    save_station_cache();
    save_ways_cache();    
    return 0;
}