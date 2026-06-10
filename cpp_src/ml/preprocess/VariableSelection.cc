#include "VariableSelection.h"

namespace ml {
    Matrix AddConstantVariable(Matrix X) {
        Matrix* result = Matrix::Zeros(X.GetRowCount(), X.GetColCount() + 1);

        for (size_t i = 0; i < X.GetRowCount(); i++) {
            for (size_t j = 0; j < X.GetColCount(); j++) {
                (*result)[i, j] = X[i, j];
            }
            (*result)[i, X.GetColCount()] = 1;
        }

        return *result;
    }
}