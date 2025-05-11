#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <iomanip>  // for setprecision

using namespace std;

using Matrix = vector<vector<float>>;

Matrix gaussianKernel(int size, float sigma) {
    Matrix kernel(size, vector<float>(size));
    float sum = 0.0;
    int half = size / 2;

    for (int i = -half; i <= half; ++i)
        for (int j = -half; j <= half; ++j) {
            float val = exp(-(i*i + j*j) / (2 * sigma * sigma));
            kernel[i + half][j + half] = val;
            sum += val;
        }

    // Normalize
    for (auto& row : kernel)
        for (auto& v : row) v /= sum;

    return kernel;
}

void blurRegion(const Matrix &image, Matrix &output, const Matrix &kernel,
                int startRow, int endRow) {
    int kSize = kernel.size();
    int kHalf = kSize / 2;
    int H = image.size();
    int W = image[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < W; ++j) {
            float acc = 0.0;
            for (int ki = -kHalf; ki <= kHalf; ++ki) {
                for (int kj = -kHalf; kj <= kHalf; ++kj) {
                    int ni = i + ki;
                    int nj = j + kj;
                    if (ni >= 0 && ni < H && nj >= 0 && nj < W) {
                        acc += image[ni][nj] * kernel[ki + kHalf][kj + kHalf];
                    }
                }
            }
            output[i][j] = acc;
        }
    }
}

Matrix gaussianBlurParallel(const Matrix &image, int kernelSize, float sigma, int numThreads) {
    Matrix kernel = gaussianKernel(kernelSize, sigma);
    Matrix output(image.size(), vector<float>(image[0].size(), 0.0));

    vector<thread> threads;
    int rowsPerThread = image.size() / numThreads;

    for (int t = 0; t < numThreads; ++t) {
        int start = t * rowsPerThread;
        int end = (t == numThreads - 1) ? image.size() : start + rowsPerThread;
        threads.emplace_back(blurRegion, cref(image), ref(output), cref(kernel), start, end);
    }

    for (auto &th : threads) th.join();

    return output;
}

int main() {
    const int SIZE = 1000;
    Matrix img(SIZE, vector<float>(SIZE));

    // Fill the image with a diagonal gradient
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            img[i][j] = (i + j) % 256;

    Matrix blurred = gaussianBlurParallel(img, 5, 1.5, 8);  // 8 threads for better performance

    cout << "Blurred Image (center 10x10 region):\n";
    for (int i = 45; i < 55; ++i) {
        for (int j = 45; j < 55; ++j)
            cout << fixed << setprecision(2) << blurred[i][j] << " ";
        cout << endl;
    }

    return 0;
}
