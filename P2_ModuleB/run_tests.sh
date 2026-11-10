
#!/bin/bash

set -e

# Thread counts to test
process_counts=(1 2 4 8 16 32 64 128 256 512)

# Compile program
gcc -o skeleton_project2moduleB skeleton_project2moduleB.c -pthread

# Run all tests
for count in "${process_counts[@]}"; do
    for run in 1 2 3; do
        echo "=== Run $run, Thread Count $count ==="
        ./skeleton_project2moduleB input.pgm output.pgm 128 "$count"
        echo ""
    done
done



