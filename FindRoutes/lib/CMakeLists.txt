include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.1
    )
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

set(CPR_BUILD_TESTS OFF CACHE BOOL "Disable CPR tests" FORCE)

add_library(tests Tests.cpp)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(tests PRIVATE cpr::cpr)