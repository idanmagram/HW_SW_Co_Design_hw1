#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

#include "utils.h"

// Generate Gaussian kernel
Matrix gaussianKernel(int size, float sigma) {
    Matrix kernel(size, Vector(size));
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
    Matrix image(size, Vector(size));

    // Fill the 10000x10000 image with some pattern values
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            image[i][j] = (i*j + i + j) % 256;  // Example pattern
    
    return image;
}


void printMatrix(const Matrix& matrix, const string& filename) {
    ofstream file(filename);

    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    file << fixed << setprecision(2);
    for (const auto& row : matrix) {
        for (int val : row) {
           file << val << " ";
        }
        file << endl;
    }
    file << endl;
    file.close();
}
