#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <iomanip>
#include <algorithm>  // for std::clamp
#include "utils.h"

// Blur a region of the image
void blurRegion(const Matrix &image, Matrix &output, const Matrix &kernel,
                int startRow, int endRow) {
    int kSize = kernel.size();
    int kHalf = kSize / 2;
    int H = image.size();
    int W = image[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < W; ++j) {
            float acc = 0.0f;
            for (int ki = 0; ki < kSize; ++ki) {

                for (int kj = 0; kj < kSize; ++kj) {
                    int ni = i + ki - kHalf;
                    int nj = j + kj - kHalf;
                    ni = clamp(ni, 0, H - 1);
                    nj = clamp(nj, 0, W - 1);
                    acc += image[ni][nj] * kernel[ki][kj];

                }
            }
            output[i][j]  = acc;
        }
    }
}

// Parallel Gaussian blur
Matrix gaussianBlurParallel(const Matrix &image, int kernelSize, float sigma, int numThreads) {
    const int H = image.size();
    const int W = image[0].size();

    Matrix kernel = gaussianKernel(kernelSize, sigma);
    Matrix output(H, vector<float>(W, 0.0f));

    vector<thread> threads;
    int rowsPerThread = H / numThreads;
    int remaining = H % numThreads;

    int currentRow = 0;
    for (int t = 0; t < numThreads; ++t) {
        int startRow = currentRow;
        int endRow = startRow + rowsPerThread + (t < remaining ? 1 : 0);
        threads.emplace_back(blurRegion, cref(image), ref(output), cref(kernel), startRow, endRow);
        currentRow = endRow;
    }

    for (auto &th : threads) th.join();
    return output;
}

int main(int argc, char* argv[]) {    
    Matrix image = generateImage(IMAGE_SIZE);

    auto blurred = gaussianBlurParallel(image, KERNEL_SIZE, KERNEL_SIGMA, NUM_THREADS);

    if (argc == 3) {
        printMatrix(image, string(argv[1]));
        printMatrix(blurred, string(argv[2]));
    }
    return 0;
}
