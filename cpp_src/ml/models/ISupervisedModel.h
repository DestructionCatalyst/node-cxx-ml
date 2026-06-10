#ifndef I_SUPERVISED_MODEL_H
#define I_SUPERVISED_MODEL_H
#include "ml/matrix.h"

namespace ml {
    class ISupervisedModel {
      public:
        virtual void Fit(Matrix X, Matrix y);
        virtual Matrix& Predict(Matrix X);
    };
}

#endif