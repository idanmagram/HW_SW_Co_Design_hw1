#include <iostream>
#include <vector>
#include <cmath>
#include <string>

constexpr int KERNEL_RADIUS = 2;
constexpr int KERNEL_SIZE = 2 * KERNEL_RADIUS + 1;
constexpr float KERNEL_SIGMA = 1.5;

using namespace std;

using Vector = vector<float>;
using Matrix = vector<Vector>;

// Generate Gaussian kernel
Matrix gaussianKernel(int kernel_size, float sigma);

Matrix generateImage(int size);

void printMatrix(const Matrix& matrix, const string& filename);

// Get 1D index for 2D coords
inline int idx(int x, int y) {
    return y * IMAGE_SIZE + x;
}