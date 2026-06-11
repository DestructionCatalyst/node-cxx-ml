#ifndef ML_METRICS_DATASET_H
#define ML_METRICS_DATASET_H
#include <vector>
#include <cmath>
#include "ml/matrix.h"

namespace ml {
    Matrix& Means(Matrix X);
    Matrix& Covariation(Matrix X);
    Matrix& Correlation(Matrix X);
}
#endif
