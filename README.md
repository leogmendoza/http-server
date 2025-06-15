# Custom Multithreaded HTTP Server in C++

## Overview

### What?
This project is a web server built without **ANY external libraries**!😱 Using only the C++ std library and the Windows Socket API, it delivers resources to clients over HTTP.  

### Why? 🗿
During my co-op at Xandar Kardian, I did a bit of backend development... but I always wondered how server calls worked at a lower level. So why _not_ create a web server that handles HTTP requests myself?

### Features
- **TCP/IP Connection Handling**  
  Manages client connections using raw system calls through the Windows Socket API   
  
- **HTTP Parsing & Routing**  
  Manually parses HTTP requests and routes them to the designated file path (ex. `/`, `about`)   

- **Static File Serving**  
  Serves HTML and PNG image files directly from the `public/` directory using standard file I/O   

- **Multithreaded Client Handling**  
  Spawns a new `std::thread` for each incoming client, allowing concurrent request handling   

- **RAII-Based Connection Tracking**  
  Tracks live client count safely using mutexes and RAII design patterns to manage shared state   

- **Custom Logger with Timestamps & Colors**  
  Prints tagged logs with ANSI colors and timestamps to help trace activity

- **Modular Architecture**  
  Organized into reusable components (`TcpServer`, `Socket`, `ClientHandler`, `Router`, `Logger`)

### Learnings
- TCP/IP

## Run It! 🙏

### Pre-reqs
- Windows (sorry Linux/Mac users 🥀)
- CMake

### Setup Instructions
1. Clone my repo:
   ```bash
   git clone https://github.com/leogmen/http-server.git
   cd http-server
2. Create build directory:
   ```bash
   mkdir build
   cd build
3. Configure project with CMake:
   ```bash
   cmake ..
5. Build executable:
   ```bash
   cmake --build .
6. Run the server!:
   ```bash
   ./http_server
7. In your browser, visit: [http://localhost:8080/](http://localhost:8080/)

## Demos/Screenshots
⚠️⚠️⚠️UNDER CONSTRUCTION⚠️⚠️⚠️

## Potential Features
lol just implementing the GET method was one too many T_T
