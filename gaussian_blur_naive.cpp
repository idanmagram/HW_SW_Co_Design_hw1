#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "utils.h"

// Apply Gaussian Blur via convolution
Matrix applyGaussianBlur(const Matrix& image, int kernelSize, float sigma) {
    int height = image.size();
    int width = image[0].size();
    int half = kernelSize / 2;
    Matrix kernel = gaussianKernel(kernelSize, sigma);
    Matrix output(height, Vector(width, 0));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float sum = 0.0;
            for (int ki = -half; ki <= half; ki++) {
                for (int kj = -half; kj <= half; kj++) {
                    int ni = i + ki;
                    int nj = j + kj;
                    ni = clamp(ni, 0, height - 1);
                    nj = clamp(nj, 0, width - 1);
                    sum += image[ni][nj] * kernel[ki + half][kj + half];
                }
            }
            output[i][j] = sum;
        }
    }

    return output;
}

int main(int argc, char* argv[]) {
    Matrix image = generateImage(IMAGE_SIZE);

    auto blurred = applyGaussianBlur(image, KERNEL_SIZE, KERNEL_SIGMA);

    if (argc == 3) {
        printMatrix(image, string(argv[1]));
        printMatrix(blurred, string(argv[2]));
    }
    return 0;
}
