cmake_minimum_required(VERSION 3.10)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

include_directories(inc)

# Find all src files and add to build
file(GLOB SRC_FILES src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    main.cpp
    ${SRC_FILES}
)

if (WIN32)
    target_link_libraries(http_server ws2_32)
endif()