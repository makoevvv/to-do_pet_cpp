cmake_minimum_required(VERSION 3.12)

project(TodoBackend CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(todo_server
    src/main.cpp
    src/database/database.cpp
    src/handlers/handlers.cpp
)

target_include_directories(todo_server PRIVATE 
    ${PQXX_INCLUDE_DIRS}
    src/include  # для httplib.h
)

target_link_libraries(todo_server PRIVATE 
    ${PQXX_LIBRARIES}
    nlohmann_json::nlohmann_json
)

install(TARGETS todo_server DESTINATION bin)