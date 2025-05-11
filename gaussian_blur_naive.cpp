#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Create a 2D Gaussian kernel
vector<vector<float>> generateGaussianKernel(int size, float sigma) {
    vector<vector<float>> kernel(size, vector<float>(size));
    float sum = 0.0;
    int half = size / 2;

    for (int x = -half; x <= half; x++) {
        for (int y = -half; y <= half; y++) {
            float value = exp(-(x*x + y*y) / (2 * sigma * sigma));
            value /= 2 * M_PI * sigma * sigma;
            kernel[x + half][y + half] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            kernel[i][j] /= sum;

    return kernel;
}

// Apply Gaussian Blur via convolution
vector<vector<float>> applyGaussianBlur(const vector<vector<float>>& image, int kernelSize, float sigma) {
    int height = image.size();
    int width = image[0].size();
    int half = kernelSize / 2;
    vector<vector<float>> kernel = generateGaussianKernel(kernelSize, sigma);
    vector<vector<float>> output(height, vector<float>(width, 0));

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
    const int SIZE = 1000;
    vector<vector<float>> image(SIZE, vector<float>(SIZE));

    // Fill the 100x100 image with gradient values
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            image[i][j] = (i + j) % 256;  // Example pattern

    int kernelSize = 5;
    float sigma = 1.5;

    auto blurred = applyGaussianBlur(image, kernelSize, sigma);

    // Print only a portion to avoid flooding the terminal
    cout << "Blurred Image (center 10x10):\n";
    for (int i = 45; i < 55; ++i) {
        for (int j = 45; j < 55; ++j) {
            printf("%6.2f ", blurred[i][j]);
        }
        cout << "\n";
    }

    return 0;
}
