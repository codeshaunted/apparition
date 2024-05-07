// codeshaunted - apparition
// include/apparition/math.hh
// contains math declarations
// Copyright 2024 codeshaunted
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org / licenses / LICENSE - 2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissionsand
// limitations under the License.

#ifndef APPARITION_MATH_HH
#define APPARITION_MATH_HH

#include <expected>
#include <initializer_list>

namespace apparition {

template<typename T, size_t C>
class MatrixRow {
    public:
        MatrixRow();
        T& getColumn(size_t j);
        T& operator[](size_t j);
    private:
        T columns[C];
};

template<typename T, size_t R, size_t C>
class Matrix {
    public:
        enum class Error {
            INVERSE_MATRIX_DOES_NOT_EXIST
        };
        Matrix();
        Matrix(std::initializer_list<std::initializer_list<T>> data);
        static Matrix<T, R, C> identity();
        T get(size_t i, size_t j);
        MatrixRow<T, C>& getRow(size_t i);
        MatrixRow<T, C>& operator[](size_t i);
        Matrix<T, R, C> rowReduce();
        Matrix<T, R - 1, C - 1> minor(size_t i, size_t j);
        Matrix<T, R, C> adjugate();
        std::expected<Matrix<T, R, C>, Error> inverse();
        T determinant();
    private:
        MatrixRow<T, C> rows[R];
};

template<typename T, size_t C>
MatrixRow<T, C>::MatrixRow() {
    static_assert(std::is_arithmetic_v<T>, "'T' must be an arithmetic type");

    if (C <= 0) {
        throw std::invalid_argument("Invalid number of columns");
    }

    for (size_t j = 0; j < C; ++j) {
        this->columns[j] = 0;
    }
}

template<typename T, size_t C>
T& MatrixRow<T, C>::getColumn(size_t j) {
    if (j < 0 || j >= C) {
        throw std::out_of_range("Value for 'j' is out of range");
    }

    return this->columns[j];
}

template<typename T, size_t C>
T& MatrixRow<T, C>::operator[](size_t j) {
    return this->getColumn(j);
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix() {
    if (R <= 0) {
        throw std::invalid_argument("Invalid number of columns");
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C>::Matrix(std::initializer_list<std::initializer_list<T>> data) {
    if (data.size() != R) {
        throw std::invalid_argument("Invalid number of rows");
    }

    size_t i = 0;
    for (const auto& row : data) {
        if (row.size() != C) {
            throw std::invalid_argument("Invalid number of columns in a row");
        }
        size_t j = 0;
        for (const auto& element : row) {
            this->rows[i][j] = element;
            ++j;
        }
        ++i;
    }
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::identity() {
    static_assert(R == C, "The identity matrix is only defined for square matrices");

    Matrix<T, R, C> result;

    for (size_t i = 0; i < R; ++i) {
        result[i][i] = 1;
    }

    return result;
}

template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::get(size_t i, size_t j) {
    return this->getRow(i)->getColumn(j);
}

template<typename T, size_t R, size_t C>
MatrixRow<T, C>& Matrix<T, R, C>::getRow(size_t i) {
    if (i < 0 || i >= R) {
        throw std::out_of_range("Value for 'i' is out of range");
    }

    return this->rows[i];
}

template<typename T, size_t R, size_t C>
MatrixRow<T, C>& Matrix<T, R, C>::operator[](size_t i) {
    return this->getRow(i);
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::rowReduce() {
    Matrix<T, R, C> result = *this;

    size_t lead = 0;
    for (size_t r = 0; r < R; ++r) {
        if (C <= lead)
            break;

        size_t i = r;
        while (result[i][lead] == 0) {
            ++i;
            if (R == i) {
                i = r;
                ++lead;
                if (C == lead)
                    break;
            }
        }

        if (C == lead)
            break;

        if (i != r) {
            std::swap(result[i], result[r]);
        }

        T val = result[r][lead];
        if (val != 0) {
            for (size_t j = 0; j < C; ++j) {
                result[r][j] /= val;
            }
        }

        for (size_t k = 0; k < R; ++k) {
            if (k != r) {
                T factor = result[k][lead];
                for (size_t j = 0; j < C; ++j) {
                    result[k][j] -= factor * result[r][j];
                }
            }
        }

        ++lead;
    }

    return result;
}

template<typename T, size_t R, size_t C>
Matrix<T, R - 1, C - 1> Matrix<T, R, C>::minor(size_t i, size_t j) {
    Matrix<T, R - 1, C - 1> minor_matrix;

    size_t minor_row = 0;
    for (size_t r = 0; r < R; ++r) {
        if (r == i) continue;
        size_t minor_column = 0;
        for (size_t c = 0; c < C; ++c) {
            if (c == j) continue;
            minor_matrix[minor_row][minor_column] = this->rows[r][c];
            ++minor_column;
        }
        ++minor_row;
    }

    return minor_matrix;
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::adjugate() {
    Matrix<T, R, C> adjugate;

    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < C; ++j) {
            Matrix<T, R - 1, C - 1> minor = this->minor(i, j);
            T cofactor = minor.determinant() * ((i + j) % 2 == 0 ? 1 : -1);
            // place the cofactor in the adjugate matrix (transposed)
            adjugate[j][i] = cofactor;
        }
    }

    return adjugate;
}

template<typename T, size_t R, size_t C>
std::expected<Matrix<T, R, C>, typename Matrix<T, R, C>::Error> Matrix<T, R, C>::inverse() {
    static_assert(R == C, "Inverse is only defined for square matrices");

    T det = this->determinant();

    // check if the determinant is zero, if so, matrix is not invertible
    if (det == 0) {
        return std::unexpected(Error::INVERSE_MATRIX_DOES_NOT_EXIST);
    }

    Matrix<T, R, C> inverse;
    Matrix<T, R, C> adjugate = this->adjugate();

    // calculate the inverse by dividing the adjugate by the determinant
    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < C; ++j) {
            inverse[i][j] = adjugate[i][j] / det;
        }
    }

    return inverse;
}

template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::determinant() {
    static_assert(R == C, "The determinant is only defined for square matrices");

    Matrix<T, R, C> reduced = this->rowReduce();

    float determinant = 1.0f;
    for (size_t i = 0; i < R; ++i) {
        determinant *= reduced[i][i];
    }

    return determinant;
}

} // namespace apparition

#endif // APPARITION_MATH_HH