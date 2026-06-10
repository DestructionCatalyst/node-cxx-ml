#ifndef TRAIN_TEST_SPLIT_H
#define TRAIN_TEST_SPLIT_H

#include "ml/matrix.h"
#include <array>
#include <optional>
#include "ml/math.h"

namespace ml {
    std::array<Matrix, 4> TrainTestSplit(Matrix X, Matrix y, float testSizeFactor);
    std::array<Matrix, 4> TrainTestSplit(Matrix X, Matrix y, float testSizeFactor, size_t seed);
    std::array<Matrix, 2> TrainTestSplit(Matrix X, float testSizeFactor);
    std::array<Matrix, 2> TrainTestSplit(Matrix X, float testSizeFactor, size_t seed);
    std::array<Matrix*, 4> TrainTestSplit(Matrix X, std::optional<Matrix> y, float testSizeFactor, std::optional<size_t> seed);
}

#endif
