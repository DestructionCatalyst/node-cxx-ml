#ifndef I_LINEARLY_TRANSFORMABLE_H
#define I_LINEARLY_TRANSFORMABLE_H

namespace ml {
    class ILinearlyTransformable {
      public:
        virtual void SwapRows(size_t row1, size_t row2);
        virtual void MultiplyRow(size_t row, float factor);
        virtual void SumRows(size_t source, size_t target, float factor = 1.f);
    };
}

#endif