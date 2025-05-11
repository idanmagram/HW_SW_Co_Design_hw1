#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <iomanip>
#include <algorithm>  // for std::clamp

using namespace std;

using Matrix = vector<vector<float>>;

// Generate Gaussian kernel
Matrix gaussianKernel(int size, float sigma) {
    Matrix kernel(size, vector<float>(size));
    float sum = 0.0f;
    int half = size / 2;

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            int x = i - half;
            int y = j - half;
            float val = exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel[i][j] = val;
            sum += val;
        }

    for (auto &row : kernel)
        for (auto &v : row) v /= sum;

    return kernel;
}

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
                /*
                for (int kj = 0; kj < kSize; ++kj) {
                    int ni = i + ki - kHalf;
                    int nj = j + kj - kHalf;
                    ni = clamp(ni, 0, H - 1);
                    nj = clamp(nj, 0, W - 1);
                    acc += image[ni][nj] * kernel[ki][kj];

                }
                 */
                for (int kj = 0; kj < kSize; ++kj) {
                    int ni = i + ki - kHalf;
                    int nj = j + kj - kHalf;

                    // Manually clamp ni and nj to valid ranges
                    if (ni < 0) ni = 0;
                    else if (ni >= H) ni = H - 1;

                    if (nj < 0) nj = 0;
                    else if (nj >= W) nj = W - 1;

                    acc += image[ni][nj] * kernel[ki][kj];
                }
            }
            output[i][j] = acc;
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

int main() {
    const int SIZE = 10000;
    Matrix img(SIZE, vector<float>(SIZE));

    // Fill the image with a simple diagonal gradient
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            img[i][j] = static_cast<float>((i + j) % 256);

    // Apply Gaussian blur
    Matrix blurred = gaussianBlurParallel(img, 5, 1.5f, 8);

    // Print center 10x10 region
    cout << "Blurred Image (center 10x10 region):\n";
    int mid = SIZE / 2;
    for (int i = mid - 5; i < mid + 5; ++i) {
        for (int j = mid - 5; j < mid + 5; ++j)
            cout << fixed << setprecision(2) << blurred[i][j] << " ";
        cout << "\n";
    }

    return 0;
}
