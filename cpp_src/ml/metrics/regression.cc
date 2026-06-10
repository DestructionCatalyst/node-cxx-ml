#include "regression.h"

namespace ml {
    size_t CheckVectorsSameLength(Matrix real, Matrix pred) {
        if (real.GetRowCount() != pred.GetRowCount()
            && real.GetColCount() != 1
            && pred.GetColCount() != 1
        ) {
            throw std::invalid_argument(
                "Real and Pred have to be 2 vectors of the same length"
            );
        }

        return real.GetRowCount();
    }

    float MeanSquaredError(Matrix real, Matrix pred) {
        size_t n = CheckVectorsSameLength(real, pred);

        Matrix diff = real - pred;

        Matrix squaredError = diff.Transpose() * diff;

        return squaredError[0, 0] / n;
    }

    float MeanAbsoluteError(Matrix real, Matrix pred) {
        size_t n = CheckVectorsSameLength(real, pred);

        Matrix diff = real - pred;

        float acc = 0;

        for (size_t i = 0; i < n; i++) {
            acc += fabs(diff[i, 0]);
        }

        return acc / n;
    }

    float RSquared(Matrix real, Matrix pred) {
        size_t n = CheckVectorsSameLength(real, pred);

        Matrix diff = real - pred;

        Matrix squaredError = diff.Transpose() * diff;

        float resSum = squaredError[0, 0];

        float sum = 0;

        for (size_t i = 0; i < n; i++) {
            sum += real[i, 0];
        }

        float mean = sum / n;

        float totSum = 0;

        for (size_t i = 0; i < n; i++) {
            float dif = real[i, 0] - mean;
            totSum += dif * dif;
        }

        return 1 - resSum / totSum;
    }

    float AdjustedRSquared(Matrix real, Matrix pred, size_t variableCount, size_t sampleCount) {
        float rSquared = RSquared(real, pred);

        return 1 - (1 - rSquared) * (sampleCount - 1) / (sampleCount - variableCount - 1);
    }

    float AdjustedRSquared(Matrix real, Matrix pred, Matrix X) {
        return AdjustedRSquared(real, pred, X.GetColCount(), X.GetRowCount());
    }

}
