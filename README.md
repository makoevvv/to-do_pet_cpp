# To-Do List Application

Полнофункциональное веб-приложение для управления задачами, построенное на современном стеке технологий.

## 🚀 Технологии

- **Backend**: C++17 с HTTP сервером на httplib
- **Database**: PostgreSQL с библиотекой libpqxx
- **Frontend**: Vanilla JavaScript, HTML5, CSS3
- **Infrastructure**: Docker, Docker Compose, Nginx
- **API**: RESTful JSON API

## 📋 Функциональность

- ✅ Создание, просмотр, редактирование и удаление задач
- ✅ Отметка задач как выполненных
- ✅ Автоматическое сохранение даты создания
- ✅ Адаптивный дизайн для мобильных устройств
- ✅ Полностью контейнеризированное приложение

## 🏗️ Архитектура
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│    Frontend     │    │    Backend      │    │     Database    │
│                 │    │                 │    │                 │
│    Nginx:80     │◄──►│   C++ API:8080  │◄──►│ PostgreSQL:5432 │
│                 │    │                 │    │                 │
│   HTML/CSS/JS   │    │     REST API    │    │   Tasks table   │
└─────────────────┘    └─────────────────┘    └─────────────────┘


## 🛠️ Быстрый старт

### Предварительные требования

- Docker
- Docker Compose
- Git

### Установка и запуск

1. **Клонируйте репозиторий**
   ```bash
   git clone https://github.com/your-username/todo-app.git
   cd todo-app
2. **Запустите приложение**
   ```bash
    docker-compose up --build
3. **Откройте в браузере**
    ```text
    http://localhost

### Остановка приложения
    ```bash
    docker-compose down

### Полная переустановка
    ```bash
    docker-compose down -v
    docker-compose up --build


## 📡 API Endpoints

### GET
  ```http
  GET /api/tasks
  
### POST
  ```http
  POST /api/tasks
  Content-Type: application/json
  
  {
    "title": "Новая задача"
  }

### PUT
  ```http
  PUT /api/tasks/{id}
  Content-Type: application/json
  
  {
    "title": "Обновленная задача",
    "completed": true
  }

### DELETE
  DELETE /api/tasks/{id}

