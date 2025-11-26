#!/bin/bash

# Compile the programs
echo "Compiling copy1.c and copy2.c..."
gcc copy1.c -o copy1 -lrt
gcc copy2.c -o copy2 -lrt

# Define buffer sizes in bytes
# 1MB, 64KB, 4KB, 64B, 1B
sizes=(1048576 65536 4096 64 1)
size_labels=("1MB" "64KB" "4KB" "64B" "1B")

# Output file
output_file="results.txt"

# Initialize results file
echo "Performance Test Results" > "$output_file"
echo "------------------------" >> "$output_file"
echo "Date: $(date)" >> "$output_file"
echo "" >> "$output_file"

# Check if input.bin exists
if [ ! -f input.bin ]; then
    echo "Error: input.bin not found."
    echo "Creating a dummy 10MB input.bin file for testing..."
    dd if=/dev/urandom of=input.bin bs=1M count=10
fi

echo "Running tests..."

# Loop through sizes
for i in "${!sizes[@]}"; do
    size=${sizes[$i]}
    label=${size_labels[$i]}

    echo "Running tests for buffer size: $label"
    echo "Buffer Size: $label ($size bytes)" >> "$output_file"
    
    # Run copy1 (System Calls)
    echo -n "  copy1 (System Calls): " >> "$output_file"
    ./copy1 input.bin output1.bin "$size" >> "$output_file"
    
    # Run copy2 (Standard I/O)
    echo -n "  copy2 (Standard I/O): " >> "$output_file"
    ./copy2 input.bin output2.bin "$size" >> "$output_file"
    
    echo "" >> "$output_file"
done

# Clean up temporary output files
rm -f output1.bin output2.bin

echo "------------------------"
echo "Tests completed. Results saved to $output_file"
cat "$output_file"
