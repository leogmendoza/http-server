cmake_minimum_required(VERSION 3.10)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

add_executable(
    ${PROJECT_NAME} 
    main.cpp
    src/socket.cpp
    src/tcp_server.cpp
    src/http_request.cpp
    src/http_response.cpp
)

if (WIN32)
    target_link_libraries(http_server ws2_32)
endif()