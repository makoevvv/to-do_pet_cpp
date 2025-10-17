const API_BASE_URL = '/api';

const tasksList = document.getElementById('tasksList');
const taskInput = document.getElementById('taskInput');
const loading = document.getElementById('loading');

// Функция для отображения/скрытия индикатора загрузки
function setLoading(isLoading) {
    loading.classList.toggle('hidden', !isLoading);
}

// Функция для обработки ошибок
function handleError(error, userMessage = 'Произошла ошибка') {
    console.error('Error:', error);
    alert(`${userMessage}: ${error.message}`);
}

// Функция для загрузки всех задач
async function loadTasks() {
    setLoading(true);
    try {
        const response = await fetch(`${API_BASE_URL}/tasks`);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const tasks = await response.json();
        displayTasks(tasks);
    } catch (error) {
        handleError(error, 'Не удалось загрузить задачи');
    } finally {
        setLoading(false);
    }
}

// Функция для отображения задач
function displayTasks(tasks) {
    tasksList.innerHTML = '';
    
    if (tasks.length === 0) {
        tasksList.innerHTML = '<p class="no-tasks">Задачи отсутствуют</p>';
        return;
    }
    
    tasks.forEach(task => {
        const taskElement = createTaskElement(task);
        tasksList.appendChild(taskElement);
    });
}

// Функция для создания HTML элемента задачи
function createTaskElement(task) {
    const taskDiv = document.createElement('div');
    taskDiv.className = `task-item ${task.completed ? 'completed' : ''}`;
    
    const formattedDate = new Date(task.created_at).toLocaleString('ru-RU');
    
    taskDiv.innerHTML = `
        <input 
            type="checkbox" 
            class="task-checkbox" 
            ${task.completed ? 'checked' : ''}
            onchange="toggleTask(${task.id}, this.checked)"
        >
        <div class="task-content">
            <div class="task-title ${task.completed ? 'completed' : ''}">
                ${escapeHtml(task.title)}
            </div>
            <div class="task-meta">
                Создано: ${formattedDate}
            </div>
        </div>
        <div class="task-actions">
            <button class="btn-delete" onclick="deleteTask(${task.id})">
                Удалить
            </button>
        </div>
    `;
    
    return taskDiv;
}

// Функция для добавления новой задачи
async function addTask() {
    const title = taskInput.value.trim();
    
    if (!title) {
        alert('Пожалуйста, введите текст задачи');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE_URL}/tasks`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ title })
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        // Очищаем поле ввода и перезагружаем список
        taskInput.value = '';
        await loadTasks();
        
    } catch (error) {
        handleError(error, 'Не удалось создать задачу');
    }
}

// Функция для переключения статуса задачи
async function toggleTask(id, completed) {
    try {
        const response = await fetch(`${API_BASE_URL}/tasks/${id}`, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ completed })
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        // Обновляем список чтобы отразить изменения
        await loadTasks();
        
    } catch (error) {
        handleError(error, 'Не удалось обновить задачу');
        // Перезагружаем список чтобы сбросить некорректное состояние
        await loadTasks();
    }
}

// Функция для удаления задачи
async function deleteTask(id) {
    if (!confirm('Вы уверены, что хотите удалить эту задачу?')) {
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE_URL}/tasks/${id}`, {
            method: 'DELETE'
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        await loadTasks();
        
    } catch (error) {
        handleError(error, 'Не удалось удалить задачу');
    }
}

// Вспомогательная функция для безопасного отображения HTML
function escapeHtml(unsafe) {
    return unsafe
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

// Обработчик нажатия Enter в поле ввода
taskInput.addEventListener('keypress', function(event) {
    if (event.key === 'Enter') {
        addTask();
    }
});

// Загружаем задачи при загрузке страницы
document.addEventListener('DOMContentLoaded', function() {
    loadTasks();
});