#include <iostream>
#include <vector>
#include <cmath>
#include "utils.h"

// Apply Gaussian Blur via convolution
Matrix applyGaussianBlur(const Matrix& image, int kernelSize, float sigma) {
    int height = image.size();
    int width = image[0].size();
    int half = kernelSize / 2;
    Matrix kernel = gaussianKernel(kernelSize, sigma);
    Matrix output(height, vector<float>(width, 0));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float sum = 0.0;
            for (int ki = -half; ki <= half; ki++) {
                for (int kj = -half; kj <= half; kj++) {
                    int ni = i + ki;
                    int nj = j + kj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                        sum += image[ni][nj] * kernel[ki + half][kj + half];
                    }
                }
            }
            output[i][j] = sum;
        }
    }

    return output;
}

int main() {
    const int kernelSize = 5;
    const float sigma = 5;

    Matrix image = generateImage(SIZE);

    auto blurred = applyGaussianBlur(image, kernelSize, sigma);

#ifdef PRINT_MODE
    // Print only a portion to avoid flooding the terminal
    cout << "Original Image (center 10x10): " << endl;
    printPartialMatrix(image, 45, 55, 45, 55);

    // Print only a portion to avoid flooding the terminal
    cout << "Blurred Image (center 10x10): " << endl;
    printPartialMatrix(blurred, 45, 55, 45, 55);
#endif

    return 0;
}
