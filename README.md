# Custom HTTP Server in C++

## Overview

### 🤔 What? 
This project is a multithreaded web server built without **ANY external libraries**!😱 Using only the C++ std library and Windows Socket API, it can respond to HTTP requests from clients and serve them content.  

### 🗿 Why? 
During my co-op at Xandar Kardian, I did a bit of backend development... but I always wondered how server calls worked at a lower level. So why _not_ create a HTTP server that connects to clients and handles requests/responses myself?

### Features
- **TCP/IP Connection Handling**:   
  Lets clients talk to the server using sockets (OSI L5) on top of TCP/IP (OSI L4/L3)
  
- **HTTP Parsing & Routing**:  
  Decodes HTTP requests and retrieves the appropriate content 

- **Static File Serving**:  
  Serves HTML files and PNG images directly from `public/`  

- **Multithreaded Client Handling**:  
  Many requests from different clients can be handled concurrently

- **Modular Design**:  
  Utilizes OOP for a clean codebase and RAII to clean up runtime resources

- **Server Logging**:  
  Logs time-stamped server activity with pretty colours >_*

## 🙏 Run It!

### Pre-reqs
- Windows (sorry Linux/Mac users 🥀)
- C++ Compiler
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

## Future Features
lol just implementing the GET method was one too many T_T
