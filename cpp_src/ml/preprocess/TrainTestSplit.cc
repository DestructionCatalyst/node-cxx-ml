#include "TrainTestSplit.h"
#include <iostream>

namespace ml {
    std::array<Matrix*, 4> TrainTestSplit(
        Matrix X,
        std::optional<Matrix> y,
        float testSizeFactor,
        std::optional<size_t> seed
    ) {
        if (y && y->GetRowCount() != X.GetRowCount()) {
             throw std::invalid_argument(std::format(
                    "Dimensions don't match: {} and {}",
                    X.GetRowCount(), X.GetRowCount()
                ));
        }

        size_t totalSize = X.GetRowCount();
        size_t testSize = std::llround(totalSize * testSizeFactor);
        size_t trainSize = totalSize - testSize;

//         std::cout << totalSize << ' ' << testSize << ' ' << trainSize << '\n';

        Matrix* xTrain = Matrix::Zeros(trainSize, X.GetColCount());
        Matrix* xTest = Matrix::Zeros(testSize, X.GetColCount());

        Matrix* yTrain = Matrix::Zeros(1, 1);
        Matrix* yTest = Matrix::Zeros(1, 1);

        if (y) {
            yTrain = Matrix::Zeros(trainSize, y->GetColCount());
            yTest = Matrix::Zeros(testSize, y->GetColCount());
        }

        std::array<Matrix*, 2> xs = { xTest, xTrain };
        std::array<Matrix*, 2> ys = { yTest, yTrain };
        std::array<size_t, 2> offsets = { 0, testSize };

        // Наполняем выборки с перемешиванием методом Fisher–Yates
        for (size_t i = 0; i < totalSize; i++) {
            // Генерим рандомное число от i до конца
            size_t j;
            if (seed) {
                j = random(i, totalSize, *seed);
            }
            else {
                j = random(i, totalSize);
            }

            // Копируем X
            int condition = i >= testSize;
            Matrix* curX = xs[condition];
            size_t offset = offsets[condition];

//             std::cout << i << ' ' << j << ' 156' << i - offset << '\n';

            for (size_t k = 0; k < X.GetColCount(); k++) {
                (*curX)[i - offset, k] = X[j, k];
            }

            // Копируем y
            if (y) {
                Matrix* curY = ys[condition];
                for (size_t k = 0; k < y->GetColCount(); k++) {
                    (*curY)[i - offset, k] = (*y)[j, k];
                }
            }
        }

        std::array<Matrix*, 4> result { xTrain, yTrain, xTest, yTest };

        return result;
    }

    std::array<Matrix, 4> TrainTestSplit(Matrix X, Matrix y, float testSizeFactor) {
        std::array<Matrix*, 4> result = TrainTestSplit(X, std::make_optional(y), testSizeFactor, std::optional<size_t>());

        Matrix returned[4] = {
            *(result[0]),
            *(result[1]),
            *(result[2]),
            *(result[3])
        };

        return std::to_array(returned);
    }
    std::array<Matrix, 4> TrainTestSplit(Matrix X, Matrix y, float testSizeFactor, size_t seed) {
        std::array<Matrix*, 4> result = TrainTestSplit(X, std::make_optional(y), testSizeFactor, std::make_optional(seed));

        Matrix returned[4] = {
            *(result[0]),
            *(result[1]),
            *(result[2]),
            *(result[3])
        };

        return std::to_array(returned);
    }
    std::array<Matrix, 2> TrainTestSplit(Matrix X, float testSizeFactor) {
        std::array<Matrix*, 4> result = TrainTestSplit(X, std::optional<Matrix>(), testSizeFactor, std::optional<size_t>());

        Matrix returned[2] = {
            *(result[0]),
            *(result[2])
        };

        return std::to_array(returned);
    }
    std::array<Matrix, 2> TrainTestSplit(Matrix X, float testSizeFactor, size_t seed) {
        std::array<Matrix*, 4> result = TrainTestSplit(X, std::optional<Matrix>(), testSizeFactor, std::make_optional(seed));

        Matrix returned[2] = {
            *(result[0]),
            *(result[2])
        };

        return std::to_array(returned);
    }
}
