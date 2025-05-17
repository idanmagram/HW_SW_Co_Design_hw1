#include <immintrin.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "utils.h"


void gaussianBlurSIMD(const Matrix& src, Matrix& dst, int kernelSize, float sigma) {
    const int H = src.size();
    const int W = src[0].size();
    Matrix kernel = gaussianKernel(kernelSize, sigma);
    int half = kernelSize / 2;

    dst.assign(H, Vector(W, 0.0f));

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; x += 8) {
            __m256 sum = _mm256_setzero_ps();

            for (int dy = -half; dy <= half; ++dy) {
                int yy = clamp(y + dy, 0, H - 1);

                for (int dx = -half; dx <= half; ++dx) {
                    float coeff = kernel[dy + half][dx + half];
                    __m256 weight = _mm256_set1_ps(coeff);

                    float buffer[8];
                    for (int i = 0; i < 8; ++i) {
                        int xx = clamp(x + i + dx, 0, W - 1);
                        buffer[i] = src[yy][xx];
                    }

                    __m256 pixels = _mm256_loadu_ps(buffer);
                    sum = _mm256_fmadd_ps(pixels, weight, sum);
                }
            }
            _mm256_storeu_ps(&dst[y][x], sum);
        }
    }
}

int main(int argc, char* argv[]) {
    Matrix image = generateImage(IMAGE_SIZE);

    Matrix blurred;
    gaussianBlurSIMD(image, blurred, KERNEL_SIZE, KERNEL_SIGMA);
    if (argc == 3) {
        printMatrix(image, string(argv[1]));
        printMatrix(blurred, string(argv[2]));
    }
    return 0;
}
