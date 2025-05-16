#!/bin/bash
SIZE=10000
g++ -Wall -Wextra -O0 -std=c++17 -g -o naive_gaussian_blur gaussian_blur_naive.cpp utils.cpp -DSIZE=$SIZE
g++ -Wall -Wextra -O0 -std=c++17 -g -o optimized_gaussian_blur gaussian_blur_optimized.cpp utils.cpp -DSIZE=$SIZE

echo "Running Naive"
time ./naive_gaussian_blur

echo "Running Optimized"
time ./optimized_gaussian_blur