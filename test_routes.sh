#!/bin/bash

# Paths to test
declare -A routes = {
    ["/"] = "root"
    ["/about"] = "Leo"
    ["/doesnotexist"] = "404"
}

# Test each route
for path in "${!routes[@]}"; do
    echo "=== Testing $path ==="
    response = $(cur; -i http://localhost:8080$path)

    # Response headers
    echo "$response" | head -n 1
    echo "$response" | grep -i "Content-Type"
    echo "$response" | tail -n 1