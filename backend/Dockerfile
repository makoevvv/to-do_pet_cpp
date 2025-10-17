# Используем официальный образ GCC (последняя стабильная версия)
FROM gcc:12

# Объяснение: Используем базовый тег gcc:12, который всегда указывает на последнюю версию GCC 12
# Это более надежно, чем указывать конкретную минорную версию

RUN apt-get update && \
    apt-get install -y \
    cmake \
    libpqxx-dev \
    postgresql-client \
    nlohmann-json3-dev \
    git \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем исходный код
COPY src/ ./src/
COPY CMakeLists.txt ./

# Скачиваем и устанавливаем httplib
RUN git clone https://github.com/yhirose/cpp-httplib.git && \
    mkdir -p src/include && \
    cp cpp-httplib/httplib.h src/include/

# Создаем директорию для сборки и собираем проект
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . --target install

EXPOSE 8080

CMD ["./build/todo_server"]