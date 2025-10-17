#pragma once
#include <pqxx/pqxx>
#include <string>
#include <nlohmann/json.hpp>

// Объяснение: Используем современную библиотеку для работы с JSON
using json = nlohmann::json;

// Структура для представления задачи
struct Task {
    int id;
    std::string title;
    bool completed;
    std::string created_at;
};

// Функции-обработчики для различных HTTP-запросов
json getAllTasks(std::shared_ptr<pqxx::connection> conn);
json createTask(std::shared_ptr<pqxx::connection> conn, const json& input);
json updateTask(std::shared_ptr<pqxx::connection> conn, int id, const json& input);
bool deleteTask(std::shared_ptr<pqxx::connection> conn, int id);

// Вспомогательные функции
Task taskFromRow(const pqxx::row& row);
json taskToJson(const Task& task);