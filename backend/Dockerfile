FROM gcc:12

RUN apt-get update && \
    apt-get install -y \
    cmake \
    libpqxx-dev \
    postgresql-client \
    nlohmann-json3-dev \
    git \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src/ ./src/
COPY CMakeLists.txt ./

RUN git clone https://github.com/yhirose/cpp-httplib.git && \
    mkdir -p src/include && \
    cp cpp-httplib/httplib.h src/include/

RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . --target install

EXPOSE 8080

CMD ["./build/todo_server"]