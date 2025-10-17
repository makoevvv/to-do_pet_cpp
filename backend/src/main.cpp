#include "httplib.h"
#include <iostream>
#include "database/database.h"
#include "handlers/handlers.h"

// Объяснение: Простая функция для установки CORS заголовков
// Это нужно чтобы фронтенд с другого порта мог обращаться к бэкенду
void setCORSHeaders(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    // Инициализируем базу данных
    auto& db = Database::getInstance();
    db.initialize();
    
    // Создаем HTTP сервер
    httplib::Server svr;
    
    // Объяснение: Обработчик для CORS preflight запросов (браузеры отправляют их перед основными)
    svr.Options(".*", [](const httplib::Request& req, httplib::Response& res) {
        setCORSHeaders(res);
        res.status = 200;
    });
    
    // GET /tasks - получение всех задач
    svr.Get("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        setCORSHeaders(res);
        try {
            auto conn = db.getConnection();
            auto tasks = getAllTasks(conn);
            res.set_content(tasks.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });
    
    // POST /tasks - создание новой задачи
    svr.Post("/tasks", [&](const httplib::Request& req, httplib::Response& res) {
        setCORSHeaders(res);
        try {
            auto input = json::parse(req.body);
            auto conn = db.getConnection();
            auto task = createTask(conn, input);
            res.status = 201;
            res.set_content(task.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });
    
    // PUT /tasks/{id} - обновление задачи
    svr.Put("/tasks/(\\d+)", [&](const httplib::Request& req, httplib::Response& res) {
        setCORSHeaders(res);
        try {
            int id = std::stoi(req.matches[1]);
            auto input = json::parse(req.body);
            auto conn = db.getConnection();
            auto task = updateTask(conn, id, input);
            res.set_content(task.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });
    
    // DELETE /tasks/{id} - удаление задачи
    svr.Delete("/tasks/(\\d+)", [&](const httplib::Request& req, httplib::Response& res) {
        setCORSHeaders(res);
        try {
            int id = std::stoi(req.matches[1]);
            auto conn = db.getConnection();
            if (deleteTask(conn, id)) {
                res.status = 204;
            } else {
                res.status = 404;
                res.set_content(json{{"error", "Task not found"}}.dump(), "application/json");
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
        }
    });
    
    std::cout << "Server starting on port 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}