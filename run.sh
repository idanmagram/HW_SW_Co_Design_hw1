#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 IMAGE_SIZE NUM_THREADS"
    exit 1
fi

IMAGE_SIZE=$1
NUM_THREADS=$2

g++ -Wall -Wextra -O2 -std=c++17 -g -o naive_gaussian_blur gaussian_blur_naive.cpp utils.cpp -DIMAGE_SIZE=$IMAGE_SIZE
g++ -Wall -Wextra -O2 -std=c++17 -pthread -g -o multithreaded_gaussian_blur gaussian_blur_multithreaded.cpp utils.cpp -DIMAGE_SIZE=$IMAGE_SIZE -DNUM_THREADS=$NUM_THREADS
# g++ -O2 -mavx -mfma -std=c++17 gaussian_blur_simd.cpp -o simd_gaussian_blur -DIMAGE_SIZE=$IMAGE_SIZE

echo "Validating Files"
./naive_gaussian_blur naive_original naive_blurred
./multithreaded_gaussian_blur multithreaded_original multithreaded_blurred

if cmp -s naive_original multithreaded_original; then
    echo "Input Images Validated"
else
    echo "Different Input Images"
    exit 1
fi

if cmp -s naive_blurred multithreaded_blurred; then
    echo "Blurred Images Validated"
else
    echo "Different Blurred Images"
    exit 1
fi

echo "Perf Naive"
perf record -e cycles,instructions,cache-references,cache-misses ./naive_gaussian_blur
perf report > naive_perf.txt

echo "Perf Multithreaded"
perf record -e cycles,instructions,cache-references,cache-misses ./optimized_gaussian_blur
perf report > threaded_perf.txt

# echo "Running SIMD"
# perf record -e cycles,instructions,cache-references,cache-misses ./simd_gaussian_blur
# perf report > threaded_perf.txt