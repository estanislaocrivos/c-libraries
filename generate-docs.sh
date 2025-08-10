#!/bin/bash

# Generate documentation for the project using Doxygen

# Check if the script is run from the correct directory
if [ ! -f "Doxyfile" ]; then
    echo "Error: Doxyfile not found. Please run this script from the directory containing the Doxyfile."
    exit 1
fi

# Run Doxygen to generate the documentation
echo "Generating documentation using Doxygen..."
doxygen Doxyfile

if [ $? -ne 0 ]; then
    echo "Error: Doxygen failed to generate documentation."
    exit 1
fi

# Open the generated documentation in the default web browser
echo "Opening documentation in web browser..."

if command -v xdg-open > /dev/null; then
    xdg-open "html/index.html"
elif command -v open > /dev/null; then
    open "docs/html/index.html"
else
    echo "Error: Unable to open documentation in web browser. Please open 'docs/html/index.html' manually."
    exit 1
fi
