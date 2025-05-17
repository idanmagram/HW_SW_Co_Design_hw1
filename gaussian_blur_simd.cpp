#include <immintrin.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "utils.h"

// // Generate a 1D Gaussian kernel
// std::vector<float> generate_gaussian_kernel(float sigma) {
//     std::vector<float> kernel(KERNEL_SIZE);
//     float sum = 0.0f;
//     for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; ++i) {
//         float val = std::exp(-(i * i) / (2.0f * sigma * sigma));
//         kernel[i + KERNEL_RADIUS] = val;
//         sum += val;
//     }
//     for (float& val : kernel) val /= sum;
//     return kernel;
// }


// // Horizontal blur using AVX
// void gaussian_blur_horizontal(const float* input, float* output, const std::vector<float>& kernel) {
//     for (int y = 0; y < SIZE; ++y) {
//         for (int x = 0; x <= SIZE - 8; x += 8) {
//             __m256 result = _mm256_setzero_ps();
//             for (int k = -KERNEL_RADIUS; k <= KERNEL_RADIUS; ++k) {
//                 int xx = std::clamp(x + k, 0, SIZE - 8); // Clamp x for safe load
//                 __m256 pixels = _mm256_loadu_ps(&input[idx(xx, y)]);
//                 __m256 weight = _mm256_set1_ps(kernel[k + KERNEL_RADIUS]);
//                 result = _mm256_fmadd_ps(pixels, weight, result);
//             }
//             _mm256_storeu_ps(&output[idx(x, y)], result);
//         }
//     }
// }

// // Vertical blur using AVX
// void gaussian_blur_vertical(const float* input, float* output, const std::vector<float>& kernel) {
//     for (int y = 0; y < SIZE; ++y) {
//         for (int x = 0; x <= SIZE - 8; x += 8) {
//             __m256 result = _mm256_setzero_ps();
//             for (int k = -KERNEL_RADIUS; k <= KERNEL_RADIUS; ++k) {
//                 int yy = std::clamp(y + k, 0, SIZE - 1);
//                 __m256 pixels = _mm256_loadu_ps(&input[idx(x, yy)]);
//                 __m256 weight = _mm256_set1_ps(kernel[k + KERNEL_RADIUS]);
//                 result = _mm256_fmadd_ps(pixels, weight, result);
//             }
//             _mm256_storeu_ps(&output[idx(x, y)], result);
//         }
//     }
// }


void gaussianBlurAVX_clamp(const Matrix& src, Matrix& dst, int kernelSize, float sigma) {
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
    // std::vector<float> input(SIZE * SIZE, 0.0f);
    // std::vector<float> temp(SIZE * SIZE, 0.0f);
    // std::vector<float> output(SIZE * SIZE, 0.0f);

    // // Example: Fill image with a diagonal gradient
    // for (int y = 0; y < SIZE; ++y)
    //     for (int x = 0; x < SIZE; ++x)
    //         input[idx(x, y)] = (float)(x + y) / (SIZE + SIZE);

    // auto kernel = generate_gaussian_kernel(1.0f);

    // gaussian_blur_horizontal(input.data(), temp.data(), kernel);
    // gaussian_blur_vertical(temp.data(), output.data(), kernel);
    Matrix image = generateImage(IMAGE_SIZE);

    Matrix blurred;
    gaussianBlurAVX_clamp(image, blurred, KERNEL_SIZE, KERNEL_SIGMA);
    if (argc == 3) {
        printMatrix(image, string(argv[1]));
        printMatrix(blurred, string(argv[2]));
    }
    return 0;
}
