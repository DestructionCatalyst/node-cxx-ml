#include "matrix.h"
#include "MatrixPair.h"
#include <iostream>
// #include "math.h"

namespace ml {
    Matrix::Matrix(size_t nRows, size_t nCols) {
        this->nRows = nRows;
        this->nCols = nCols;
        this->data = std::vector<float>();
        this->data.reserve(nRows * nCols);
    }

    Matrix::Matrix(Matrix& other) {
        this->nRows = other.nRows;
        this->nCols = other.nCols;
        this->data = std::vector<float>(other.data);
    }

    Matrix* Matrix::Zeros(size_t nRows, size_t nCols) {
        Matrix* matrix = new Matrix(nRows, nCols);

        matrix->data.assign(nRows * nCols, 0.f);

        return matrix;
    }

    Matrix* Matrix::Identity(size_t dim) {
        Matrix* matrix = Matrix::Zeros(dim, dim);

        for (size_t i = 0; i < dim; i++) {
            matrix->data[i * dim + i] = 1;
        }

        return matrix;
    }

    size_t Matrix::GetRowCount() {
        return this->nRows;
    }

    size_t Matrix::GetColCount() {
        return this->nCols;
    }

    bool Matrix::IsSquare() {
        return this->nRows == this->nCols;
    }

    size_t Matrix::GetDim() {
        if (!this->IsSquare()) {
            throw std::invalid_argument(std::format(
                "GetDim only available for square matrices, not ({}x{})",
                this->nRows, this->nCols
            ));
        }

        return this->nRows;
    }

    Matrix& Matrix::operator+(Matrix other) {
        if (this->nRows != other.nRows || this->nCols != other.nCols) {
            throw std::invalid_argument(std::format(
                "Cannot add matrices with different dimensions: ({}x{}) and ({}x{})",
                this->nRows, this->nCols, other.nRows, other.nCols
            ));
        }

        Matrix* result = Matrix::Zeros(this->nRows, this->nCols);

        for (size_t row = 0; row < this->nRows; row++) {
            for (size_t col = 0; col < this->nCols; col++) {
                (*result)[row, col] = ((*this)[row, col]) + (other[row, col]);
            }
        }

        return *result;
    }

    Matrix& Matrix::operator-(Matrix other) {
        if (this->nRows != other.nRows || this->nCols != other.nCols) {
            throw std::invalid_argument(std::format(
                "Cannot subtract matrices with different dimensions: ({}x{}) and ({}x{})",
                this->nRows, this->nCols, other.nRows, other.nCols
            ));
        }

        Matrix* result = Matrix::Zeros(this->nRows, this->nCols);

        for (size_t row = 0; row < this->nRows; row++) {
            for (size_t col = 0; col < this->nCols; col++) {
                ((*result)[row, col]) = ((*this)[row, col]) - (other[row, col]);
            }
        }

        return *result;
    }

    Matrix& Matrix::operator*(float factor) {
        Matrix* result = Matrix::Zeros(this->nRows, this->nCols);

        for (size_t row = 0; row < this->nRows; row++) {
            for (size_t col = 0; col < this->nCols; col++) {
                ((*result)[row, col]) = ((*this)[row, col]) * factor;
            }
        }

        return *result;
    }

    Matrix& Matrix::operator*(Matrix other) {
        if (this->nCols != other.nRows) {
            throw std::invalid_argument(std::format(
                "Cannot multiply matrices ({}x{}) and ({}x{}): {} != {}",
                this->nRows, this->nCols, other.nRows, other.nCols, this->nCols, other.nRows
            ));
        }

        Matrix* result = Matrix::Zeros(this->nRows, other.nCols);

        for (size_t row = 0; row < this->nRows; row++) {
            for (size_t col = 0; col < other.nCols; col++) {
                float current = 0;

                for (size_t i = 0; i < this->nCols; i++) {
                    current += (*this)[row, i] * other[i, col];
                }

                ((*result)[row, col]) = current;
            }
        }

        return *result;
    }

    float& Matrix::operator[](size_t row, size_t column) {
        if (row > nRows || column > nCols) {
            throw std::out_of_range(std::format(
                "Matrix: row {} of {}, column {} of {}",
                row, nRows, column, nCols
            ));
        }

        return this->data[row * this->nCols + column];
    }

    Matrix& Matrix::Transpose() {
        Matrix* result = Matrix::Zeros(this->nCols, this->nRows);

        for (size_t row = 0; row < this->nRows; row++) {
            for (size_t col = 0; col < this->nCols; col++) {
                ((*result)[col, row]) = ((*this)[row, col]);
            }
        }

        return *result;
    }

    void Matrix::SwapRows(size_t row1, size_t row2) {
        float tmp;

        for (size_t col = 0; col < this->nCols; col++) {
            tmp = (*this)[row1, col];
            (*this)[row1, col] = (*this)[row2, col];
            (*this)[row2, col] = tmp;
        }
    }

    void Matrix::MultiplyRow(size_t row, float factor) {
        for (size_t col = 0; col < this->nCols; col++) {
            (*this)[row, col] = ((*this)[row, col]) * factor;
        }
    }

     void Matrix::SumRows(size_t source, size_t target, float factor) {
        for (size_t col = 0; col < this->nCols; col++) {
            (*this)[target, col] = ((*this)[target, col]) + ((*this)[source, col]) * factor;
        }
    }

    Matrix& Matrix::Inverse() {
        if (!this->IsSquare()) {
            throw std::invalid_argument(std::format(
                "Inverse of a non-square matrix ({}x{})",
                this->nRows, this->nCols
            ));
        }

        Matrix* identity = Matrix::Identity(this->GetDim());
        MatrixPair gaussTarget = MatrixPair(*this, *identity);

        // FORWARD
        for (size_t i = 0; i < this->GetDim(); i++) {
            // Search for non-zero row
            size_t nonZeroRow = -1u;

            for (size_t j = i; j < this->GetDim(); j++) {
                if (fabs(gaussTarget.first[j, i]) > EPSILON) {
//                     std::cout << "row " << j << '\n';
                    nonZeroRow = j;
                    break;
                }
            }

            // if not found, no inverse matrix
            if (nonZeroRow == -1u) {
                throw std::invalid_argument(std::format(
                    "Inverse of a degenerate matrix"
                ));
            }

            // Put it first
            if (nonZeroRow != i) {
                gaussTarget.SwapRows(i, nonZeroRow);
            }

            // Normalize by first element
            gaussTarget.MultiplyRow(i, 1 / gaussTarget.first[i, i]);

            // Subtract from all rows below with respective coefficients
            for (size_t j = i + 1; j < this->GetDim(); j++) {
                gaussTarget.SumRows(i, j, - gaussTarget.first[j, i]);
            }
        }

        // BACKWARD
        for (size_t i = this->GetDim() - 1; i > 0; i--) {
            for (size_t j = 0; j < i; j++) {
                gaussTarget.SumRows(i, j, - gaussTarget.first[j, i]);
            }
        }

        Matrix* toReturn = new Matrix(gaussTarget.second);

        return *toReturn;
    }

    Matrix& Matrix::Project(
        std::optional<std::vector<size_t>> rows,
        std::optional<std::vector<size_t>> cols,
        ProjectMode rowMode,
        ProjectMode colMode
    ) {
        std::vector<size_t> includedRows;

        if (!rows || rowMode == ProjectMode::EXCLUDE) {
            includedRows = std::vector<size_t>();

            for (size_t i = 0; i < this->nRows; i++) {
                if (!rows || std::find(rows->begin(), rows->end(), i) == rows->end()) {
                    includedRows.push_back(i);
                }
            }
        }
        else {
            includedRows = *rows;
        }

        std::vector<size_t> includedCols;

        if (!cols || colMode == ProjectMode::EXCLUDE) {
            includedCols = std::vector<size_t>();

            for (size_t i = 0; i < this->nCols; i++) {
                if (!cols || std::find(cols->begin(), cols->end(), i) == cols->end()) {
                    includedCols.push_back(i);
                }
            }
        }
        else {
            includedCols = *cols;
        }

        Matrix* result = Matrix::Zeros(includedRows.size(), includedCols.size());

        for (size_t row = 0; row < result->nRows; row++) {
            for (size_t col = 0; col < result->nCols; col++) {
                (*result)[row, col] = (*this)[includedRows[row], includedCols[col]];
            }
        }

        return *result;
    }

    Matrix::~Matrix() {}
}
