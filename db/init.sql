-- Объяснение: Этот скрипт создает таблицу 'tasks' с необходимыми полями.
-- id - первичный ключ, автоматически увеличивающийся.
-- title - текст задачи, не может быть NULL.
-- completed - статус выполнения, по умолчанию FALSE.
-- created_at - метка времени создания, проставляется автоматически.

CREATE TABLE IF NOT EXISTS tasks (
    id SERIAL PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    completed BOOLEAN NOT NULL DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);