#include <iostream>
#include <vector>
#include <cmath>
#include "utils.h"

// Generate Gaussian kernel
Matrix gaussianKernel(int size, float sigma) {
    Matrix kernel(size, vector<float>(size));
    float sum = 0.0f;
    int half = size / 2;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x = i - half;
            int y = j - half;
            float val = exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel[i][j] = val;
            sum += val;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] = kernel[i][j] / sum;
        }
    }

    return kernel;
}

Matrix generateImage(int size) {
    Matrix image(size, vector<float>(size));

    // Fill the 10000x10000 image with some pattern values
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            image[i][j] = (i*j + i + j) % 256;  // Example pattern
    
    return image;
}


void printPartialMatrix(const Matrix& matrix, int xmin, int xmax, int ymin, int ymax) {
    cout << "[";
    for (int i = xmin; i < xmax; ++i) {
        cout << "[";
        for (int j = ymin; j < ymax; ++j) {
            printf("%6.2f, ", matrix[i][j]);
        }
        cout << "]," << endl;
    }
    cout << "]" << endl;
}