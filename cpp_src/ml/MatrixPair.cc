#include "MatrixPair.h"

namespace ml {
    MatrixPair::MatrixPair(Matrix matrix1, Matrix matrix2) :
        std::pair<Matrix, Matrix>(matrix1, matrix2) {}

    void MatrixPair::SwapRows(size_t row1, size_t row2) {
        this->first.SwapRows(row1, row2);
        this->second.SwapRows(row1, row2);
    }

    void MatrixPair::MultiplyRow(size_t row, float factor) {
        this->first.MultiplyRow(row, factor);
        this->second.MultiplyRow(row, factor);
    }

     void MatrixPair::SumRows(size_t source, size_t target, float factor) {
        this->first.SumRows(source, target, factor);
        this->second.SumRows(source, target, factor);
    }
}