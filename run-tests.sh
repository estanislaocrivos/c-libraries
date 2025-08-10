#!/bin/bash

# Run all tests inside test directory

# Check if the script is run from the correct directory
if [ ! -f "project.yml" ]; then
    echo "Error: project.yaml not found. Please run this script from the directory containing the project.yaml."
    exit 1
fi

# Run the tests using the test runner
echo "Running tests..."

ceedling test:all

echo "Running tests coverage report..."
ceedling gcov:all
