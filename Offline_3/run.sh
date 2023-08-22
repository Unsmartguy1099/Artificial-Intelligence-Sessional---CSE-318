#!/bin/bash

# Compile all Java classes in the mypackage package
javac -cp src src/mypackage/*.java

# Function to run Main for an input
run_main() {
    java -cp src mypackage.Main "$1"
}

# Define the maximum number of concurrent processes
MAX_CONCURRENT=4
CURRENT_CONCURRENT=0

# Run Main concurrently for inputs g10 to g54
for input in {1..54}; do
    # Check the number of currently running concurrent processes
    if [ "$CURRENT_CONCURRENT" -lt "$MAX_CONCURRENT" ]; then
        run_main "g$input" &  # Start a new process in the background
        ((CURRENT_CONCURRENT++))  # Increment the count of running processes
    else
        wait -n  # Wait for any background process to finish
        ((CURRENT_CONCURRENT--))  # Decrement the count of running processes
        run_main "g$input" &  # Start a new process in the background
        ((CURRENT_CONCURRENT++))  # Increment the count of running processes
    fi
done

# Wait for all remaining background jobs to finish
wait

# Change to the outputfiles directory
cd outputfiles

output_file="gall.csv"

# Clear output file if it exists
> "$output_file"

# Loop through g1output.txt to g55output.txt and append their content to gall.csv
for input in {1..55}; do
    input_file="g${input}output.txt"
    if [ -f "$input_file" ]; then
        # Replace spaces with commas and append to the CSV file
        sed 's/ /,/g' "$input_file" >> "$output_file"
        echo >> "$output_file"  # Insert a new line
    else
        echo "File $input_file not found."
    fi
done

echo "Merged all files into $output_file"