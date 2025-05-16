#!/bin/bash
SIZE=16384
g++ -Wall -Wextra -O2 -std=c++17 -g -o naive_gaussian_blur gaussian_blur_naive.cpp utils.cpp -DSIZE=$SIZE
g++ -Wall -Wextra -O2 -std=c++17 -g -o optimized_gaussian_blur gaussian_blur_optimized.cpp utils.cpp -DSIZE=$SIZE
g++ -O2 -mavx -mfma -std=c++17 gaussian_blur_simd.cpp -o simd_gaussian_blur

# echo "Running Naive"
# time ./naive_gaussian_blur

echo "Running Optimized"
perf record -a -g ./optimized_gaussian_blur
perf 
# perf script report flamegraph
# firefox flamegraph.html

# echo "Running SIMD"
# time ./simd_gaussian_blur
