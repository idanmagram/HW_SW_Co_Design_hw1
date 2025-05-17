#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 IMAGE_SIZE NUM_THREADS"
    exit 1
fi

IMAGE_SIZE=$1
NUM_THREADS=$2

PERF_FLAGS="-e task-clock,cycles,instructions,cache-references,cache-misses -a -F 1000 -g"
# REPORT_FLAMEGRAPH=1
g++ -Wall -Wextra -O2 -std=c++17 -g -o naive_gaussian_blur gaussian_blur_naive.cpp utils.cpp -DIMAGE_SIZE=$IMAGE_SIZE
g++ -Wall -Wextra -O2 -std=c++17 -pthread -g -o multithreaded_gaussian_blur gaussian_blur_multithreaded.cpp utils.cpp -DIMAGE_SIZE=$IMAGE_SIZE -DNUM_THREADS=$NUM_THREADS
g++ -O2 -mavx -mfma -std=c++17 -g -o simd_gaussian_blur gaussian_blur_simd.cpp utils.cpp -DIMAGE_SIZE=$IMAGE_SIZE

echo "Validating Files"
./naive_gaussian_blur naive_original naive_blurred
./multithreaded_gaussian_blur multithreaded_original multithreaded_blurred
./simd_gaussian_blur simd_original simd_blurred

if cmp -s naive_original multithreaded_original && cmp -s naive_original simd_original; then
    echo "Input Images Validated"
else
    echo "Different Input Images"
    exit 1
fi

if cmp -s naive_blurred multithreaded_blurred && cmp -s naive_blurred simd_blurred; then
    echo "Blurred Images Validated"
else
    echo "Different Blurred Images"
    exit 1
fi

echo "Perf Naive"
perf record $PERF_FLAGS ./naive_gaussian_blur
if [ -n "$REPORT_FLAMEGRAPH" ]; then
    perf script > naive.perf
    ../Flamegraph/stackcollapse-perf.pl naive.perf > naive.folded
    ../Flamegraph/flamegraph.pl naive.folded > naive_flamegraph.svg
    # xdg-open naive_flamegraph.svg &
else
    perf report > "naive_perf_$(echo $IMAGE_SIZE).txt"
fi

echo "Perf Multithreaded"
perf record $PERF_FLAGS ./multithreaded_gaussian_blur
if [ -n "$REPORT_FLAMEGRAPH" ]; then
    perf script > multithreaded.perf
    ../Flamegraph/stackcollapse-perf.pl multithreaded.perf > multithreaded.folded
    ../Flamegraph/flamegraph.pl multithreaded.folded > multithreaded_flamegraph.svg
    # xdg-open multithreaded_flamegraph.svg &
else
    perf report > "multithreaded_perf_$(echo $IMAGE_SIZE)_$(echo $NUM_THREADS).txt"
fi

echo "Running SIMD"
perf record $PERF_FLAGS ./simd_gaussian_blur
if [ -n "$REPORT_FLAMEGRAPH" ]; then
    perf script > simd.perf
    ../Flamegraph/stackcollapse-perf.pl simd.perf > simd.folded
    ../Flamegraph/flamegraph.pl simd.folded > simd_flamegraph.svg
    # xdg-open simd_flamegraph.svg &
else
    perf report > "simd_perf_$(echo $IMAGE_SIZE).txt"
fi
