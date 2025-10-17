#pragma once
#include <pqxx/pqxx>
#include <string>
#include <memory>

// Класс-обертка для работы с подключением к БД
// Используем паттерн Singleton для гарантии единственного подключения
class Database {
public:
    static Database& getInstance();
    
    // Метод для получения подключения
    std::shared_ptr<pqxx::connection> getConnection();
    
    // Метод для инициализации таблиц (на всякий случай)
    void initialize();

private:
    Database(); // Приватный конструктор для Singleton
    std::string connection_string_;
};