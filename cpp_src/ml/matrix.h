#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <format>
#include <stdexcept>
#include <algorithm>
#include "ILinearlyTransformable.h"
#include "math.h"

namespace ml {
    enum class ProjectMode {
        INCLUDE,
        EXCLUDE
    };

    class Matrix: public ILinearlyTransformable {
    private:
        std::vector<float> data;
        size_t nRows;
        size_t nCols;
    public:
        Matrix(size_t nRows, size_t nCols);
        Matrix(Matrix& other);
        // Factory methods
        static Matrix* Zeros(size_t nRows, size_t nCols);
        static Matrix* Identity(size_t dim);

        size_t GetRowCount();
        size_t GetColCount();
        bool IsSquare();
        size_t GetDim();

        float& operator [](size_t row, size_t column);
//         const float& operator [](size_t row, size_t column) const;
        Matrix& operator +(Matrix other);
        Matrix& operator -(Matrix other);
        Matrix& operator *(float factor);
        Matrix& operator *(Matrix other);

        Matrix& Transpose();
        Matrix& Inverse();
        Matrix& Project(
            std::optional<std::vector<size_t>> rows,
            std::optional<std::vector<size_t>> cols,
            ProjectMode rowMode,
            ProjectMode colMode
        );

        virtual ~Matrix();

        virtual void SwapRows(size_t row1, size_t row2);
        virtual void MultiplyRow(size_t row, float factor);
        virtual void SumRows(size_t source, size_t target, float factor = 1.f);
    };
}

#endif
