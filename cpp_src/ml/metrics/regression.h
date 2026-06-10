#ifndef ML_METRICS_REGRESSION_H
#define ML_METRICS_REGRESSION_H
#include "ml/matrix.h"
#include "ml/math.h"

namespace ml {
    float MeanSquaredError(Matrix real, Matrix pred);
    float MeanAbsoluteError(Matrix real, Matrix pred);
    float RSquared(Matrix real, Matrix pred);
    float AdjustedRSquared(Matrix real, Matrix pred, size_t variableCount, size_t sampleCount);
    float AdjustedRSquared(Matrix real, Matrix pred, Matrix X);
}
#endif
