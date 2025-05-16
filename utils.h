#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

using Matrix = vector<vector<float>>;

// Generate Gaussian kernel
Matrix gaussianKernel(int size, float sigma);

Matrix generateImage(int size);

void printPartialMatrix(const Matrix& matrix, int xmin, int xmax, int ymin, int ymax);
