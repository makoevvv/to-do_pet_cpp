#include "handlers.h"
#include <iostream>

Task taskFromRow(const pqxx::row& row) {
    Task task;
    task.id = row["id"].as<int>();
    task.title = row["title"].as<std::string>();
    task.completed = row["completed"].as<bool>();
    task.created_at = row["created_at"].as<std::string>();
    return task;
}

json taskToJson(const Task& task) {
    return {
        {"id", task.id},
        {"title", task.title},
        {"completed", task.completed},
        {"created_at", task.created_at}
    };
}

json getAllTasks(std::shared_ptr<pqxx::connection> conn) {
    try {
        pqxx::work txn(*conn);
        auto result = txn.exec("SELECT id, title, completed, created_at FROM tasks ORDER BY created_at DESC");
        
        json tasks = json::array();
        for (const auto& row : result) {
            tasks.push_back(taskToJson(taskFromRow(row)));
        }
        
        txn.commit();
        return tasks;
        
    } catch (const std::exception& e) {
        std::cerr << "Error getting tasks: " << e.what() << std::endl;
        throw;
    }
}

json createTask(std::shared_ptr<pqxx::connection> conn, const json& input) {
    try {
        if (!input.contains("title") || input["title"].empty()) {
            throw std::runtime_error("Title is required");
        }
        
        std::string title = input["title"];
        pqxx::work txn(*conn);
        
        auto result = txn.exec_params(
            "INSERT INTO tasks (title) VALUES ($1) RETURNING id, title, completed, created_at",
            title
        );
        
        txn.commit();
        
        if (result.empty()) {
            throw std::runtime_error("Failed to create task");
        }
        
        return taskToJson(taskFromRow(result[0]));
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating task: " << e.what() << std::endl;
        throw;
    }
}

json updateTask(std::shared_ptr<pqxx::connection> conn, int id, const json& input) {
    try {
        pqxx::work txn(*conn);
        
        // Динамически строим запрос на основе переданных полей
        std::string query = "UPDATE tasks SET ";
        std::vector<std::string> updates;
        
        if (input.contains("title")) {
            updates.push_back("title = " + txn.quote(input["title"].get<std::string>()));
        }
        if (input.contains("completed")) {
            updates.push_back("completed = " + txn.quote(input["completed"].get<bool>()));
        }
        
        if (updates.empty()) {
            throw std::runtime_error("No fields to update");
        }
        
        query += updates.front();
        for (size_t i = 1; i < updates.size(); ++i) {
            query += ", " + updates[i];
        }
        
        query += " WHERE id = " + txn.quote(id);
        query += " RETURNING id, title, completed, created_at";
        
        auto result = txn.exec(query);
        txn.commit();
        
        if (result.empty()) {
            throw std::runtime_error("Task not found");
        }
        
        return taskToJson(taskFromRow(result[0]));
        
    } catch (const std::exception& e) {
        std::cerr << "Error updating task: " << e.what() << std::endl;
        throw;
    }
}

bool deleteTask(std::shared_ptr<pqxx::connection> conn, int id) {
    try {
        pqxx::work txn(*conn);
        auto result = txn.exec_params(
            "DELETE FROM tasks WHERE id = $1",
            id
        );
        
        txn.commit();
        return result.affected_rows() > 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error deleting task: " << e.what() << std::endl;
        throw;
    }
}