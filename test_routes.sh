#!/bin/bash

# Paths to test
declare -A routes = {
    ["/"] = "root"
    ["/about"] = "Leo"
    ["/unknown"] = "404"
}