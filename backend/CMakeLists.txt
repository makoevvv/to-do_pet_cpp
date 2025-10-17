# Указываем минимальную требуемую версию CMake
cmake_minimum_required(VERSION 3.12)

# Задаем имя проекта и используемый стандарт C++
project(TodoBackend CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Объяснение: Находим необходимые библиотеки через pkg-config
find_package(PkgConfig REQUIRED)

# Ищем libpqxx
pkg_check_modules(PQXX REQUIRED libpqxx)

# Ищем nlohmann-json (если установлено через пакетный менеджер)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Добавляем исполняемый файл из исходников
add_executable(todo_server
    src/main.cpp
    src/database/database.cpp
    src/handlers/handlers.cpp
)

# Объяснение: Подключаем зависимости к нашему исполняемому файлу
target_include_directories(todo_server PRIVATE 
    ${PQXX_INCLUDE_DIRS}
    src/include  # для httplib.h
)

target_link_libraries(todo_server PRIVATE 
    ${PQXX_LIBRARIES}
    nlohmann_json::nlohmann_json
)

# Указываем директорию для установки бинарника
install(TARGETS todo_server DESTINATION bin)