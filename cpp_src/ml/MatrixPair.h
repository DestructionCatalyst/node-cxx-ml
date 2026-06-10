#ifndef MATRIX_PAIR_H
#define MATRIX_PAIR_H

#include <utility>
#include "matrix.h"
#include "ILinearlyTransformable.h"

namespace ml {
    class MatrixPair:
        public std::pair<Matrix, Matrix>,
        public ILinearlyTransformable
    {
    public:
        MatrixPair(Matrix matrix1, Matrix matrix2);

        virtual void SwapRows(size_t row1, size_t row2);
        virtual void MultiplyRow(size_t row, float factor);
        virtual void SumRows(size_t source, size_t target, float factor = 1.f);
    };
}

#endif