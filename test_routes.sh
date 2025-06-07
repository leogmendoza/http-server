#!/bin/bash

# Paths to test
declare -A routes=(
    ["/"]="root"
    ["/about"]="leo"
    ["/doesnotexist"]="404"
)

# Test each route
for path in "${!routes[@]}"; do
    echo "=== Testing: $path ==="
    response=$(curl -s -i http://localhost:8080$path)

    # Response headers
    echo "$response" | head -n 1
    echo "$response" | grep -i "Content-Type"
    echo "$response" | tail -n 1

    # Check for expected keyword
    expected=${routes[$path]}

    if echo "$response" | grep -iq "$expected"; then
        echo "[PASS] Found expected text: $expected"
    else
        echo "[FAIL] Did not find expected text: $expected"
    fi

    echo ""

done