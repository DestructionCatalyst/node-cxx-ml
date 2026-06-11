#include "dataset.h"

namespace ml {
    Matrix& Means(Matrix X) {
        size_t rows = X.GetRowCount();
        size_t cols = X.GetColCount();
        Matrix* result = Matrix::Zeros(1, cols);

        for (size_t i = 0; i < cols; i++) {
            float acc = 0;
            for (size_t j = 0; j < rows; j++) {
                acc += X[j, i];
            }
            (*result)[0, i] = acc / rows;
        }

        return *result;
    }
    Matrix& Covariation(Matrix X) {
        size_t rows = X.GetRowCount();
        size_t cols = X.GetColCount();
        Matrix* result = Matrix::Zeros(cols, cols);
        Matrix means = Means(X);

        for (size_t i = 0; i < cols; i++) {
            float meanI = means[0, i];
            for (size_t j = 0; j < cols; j++) {
                float meanJ = means[0, j];

                float acc = 0;
                for (size_t k = 0; k < rows; k++) {
                    acc += (X[k, i] - meanI) * (X[k, j] - meanJ);
                }

                (*result)[i, j] = acc / rows;
            }
        }

        return *result;
    }
    Matrix& Correlation(Matrix X) {
        size_t cols = X.GetColCount();
        Matrix cov = Covariation(X);
        Matrix* result = Matrix::Zeros(cols, cols);

        std::vector<float> inverseSigmas = std::vector<float>();
        inverseSigmas.reserve(cols);

        for (size_t i = 0; i < cols; i++) {
            inverseSigmas[i] = 1 / std::sqrtf(cov[i, i]);
        }

        for (size_t i = 0; i < cols; i++) {
            for (size_t j = 0; j < cols; j++) {
                (*result)[i, j] = cov[i, j] * inverseSigmas[i] * inverseSigmas[j];
            }
        }

        return *result;
    }
}
