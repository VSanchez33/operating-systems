
#!/bin/bash

set -e

process_counts=(1 2 4 8 16 32 64 128 256 512 1024)

for count in "${process_counts[@]}"; do
    for run in 1 2 3; do
        echo "=== Run $run, Process Count $count ==="
        ./parent "$count"
        echo ""
    done
done