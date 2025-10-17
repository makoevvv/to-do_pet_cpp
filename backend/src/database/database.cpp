#include "database.h"
#include <iostream>

Database::Database() {
    // Получаем строку подключения из переменной окружения
    // или используем значение по умолчанию
    const char* db_url = std::getenv("DATABASE_URL");
    if (db_url) {
        connection_string_ = db_url;
    } else {
        connection_string_ = "postgresql://todo_user:todo_pass@localhost:5432/todo_db";
    }
    std::cout << "Database connection string: " << connection_string_ << std::endl;
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

std::shared_ptr<pqxx::connection> Database::getConnection() {
    try {
        auto conn = std::make_shared<pqxx::connection>(connection_string_);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to open database connection");
        }
        return conn;
    } catch (const std::exception& e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        throw;
    }
}

void Database::initialize() {
    // Таблица уже создана скриптом init.sql, но это резервная инициализация
    auto conn = getConnection();
    pqxx::work txn(*conn);
    
    txn.exec(
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id SERIAL PRIMARY KEY, "
        "title VARCHAR(255) NOT NULL, "
        "completed BOOLEAN NOT NULL DEFAULT FALSE, "
        "created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP)"
    );
    
    txn.commit();
    std::cout << "Database initialized successfully" << std::endl;
}