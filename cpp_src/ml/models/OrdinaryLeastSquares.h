#ifndef ORDINARY_LEAST_SQUARES_H
#define ORDINARY_LEAST_SQUARES_H
#include "ml/matrix.h"
#include "ISupervisedModel.h"

namespace ml {
    class OrdinaryLeastSquares: public ISupervisedModel {
      public:
        OrdinaryLeastSquares();
        virtual void Fit(Matrix X, Matrix y);
        virtual Matrix& Predict(Matrix X);
      private:
        Matrix* coefficients;
    };
}

#endif