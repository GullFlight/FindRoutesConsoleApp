add_executable(labwork6 main.cpp)

target_link_libraries(labwork6 PRIVATE tests)
target_link_libraries(labwork6 PRIVATE cpr)
target_link_libraries(labwork6 PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(labwork6 PRIVATE ${PROJECT_SOURCE_DIR})