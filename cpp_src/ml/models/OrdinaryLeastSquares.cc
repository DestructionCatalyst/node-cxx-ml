#include "OrdinaryLeastSquares.h"

namespace ml {
    OrdinaryLeastSquares::OrdinaryLeastSquares() {
        this->coefficients = nullptr;
    }

    void OrdinaryLeastSquares::Fit(Matrix X, Matrix y) {
        Matrix X_T = X.Transpose();

        Matrix coefficients = (X_T * X).Inverse() * X_T * y;
        this->coefficients = new Matrix(coefficients);
    }

    Matrix& OrdinaryLeastSquares::Predict(Matrix X) {
        if (this->coefficients == nullptr) {
            throw std::logic_error("Called Predict on an untrained model. Call Fit first");
        }

        return X * (*this->coefficients);
    }
}
