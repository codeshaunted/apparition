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

#include <cmath>
#include <expected>
#include <initializer_list>
#include <stdexcept>

namespace apparition {

template<typename T, size_t N>
class Vector {
    public:
        Vector();
        Vector(std::initializer_list<T> data);
        T& get(size_t i);
        T& operator[](size_t i);
        T length();
        T dot(Vector<T, N>& other);
    protected:
        T data[N];
};

template<typename T, size_t N>
Vector<T, N>::Vector() {
    static_assert(std::is_arithmetic_v<T>, "'T' must be an arithmetic type");

    if (N <= 0) {
        throw std::invalid_argument("Invalid number of elements");
    }

    for (size_t j = 0; j < N; ++j) {
        this->data[j] = 0;
    }
}

template<typename T, size_t N>
Vector<T, N>::Vector(std::initializer_list<T> data) {
    if (data.size() != N) {
        throw std::invalid_argument("Invalid number of elements");
    }

    size_t i = 0;
    for (const auto& element : data) {
        this->data[i] = element;
        ++i;
    }
}

template<typename T, size_t N>
T& Vector<T, N>::get(size_t i) {
    if (i < 0 || i >= N) {
        throw std::out_of_range("Value for 'i' is out of range");
    }

    return this->data[i];
}

template<typename T, size_t N>
T& Vector<T, N>::operator[](size_t i) {
    return this->get(i);
}

template <typename T, size_t N>
T Vector<T, N>::length() {
    T squares_sum = 0;
    for (size_t i = 0; i < N; ++i) {
        squares_sum += this->data[i] * this->data[i];
    }

    return std::sqrt(squares_sum);
}

template<typename T, size_t N>
T Vector<T, N>::dot(Vector<T, N>& other) {
    T product = 0;

    for (size_t i = 0; i < N; ++i) {
        product += this->data[i] * other.data[i];
    }

    return product;
}


template<typename T>
class Vector2 : public Vector<T, 2> {
    public:
        T& x = Vector<T, 2>::data[0];
        T& y = Vector<T, 2>::data[1];
        Vector2() : Vector<T, 2>() {}
        Vector2(T _x, T _y) : Vector<T, 2>({_x, _y}) {}
        Vector2(const Vector<T, 2>& other) : Vector<T, 2>(other) {}
        Vector2<T>& operator=(const Vector2<T>& other);
};

template<typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other) {
    if (this != &other) {
        Vector<T, 2>::operator=(other);
    }

    return *this;
}

template<typename T>
class Vector3 : public Vector<T, 3> {
    public:
        T& x = Vector<T, 3>::data[0];
        T& y = Vector<T, 3>::data[1];
        T& z = Vector<T, 3>::data[2];
        T& r = Vector<T, 3>::data[0];
        T& g = Vector<T, 3>::data[1];
        T& b = Vector<T, 3>::data[2];
        Vector3() : Vector<T, 3>() {}
        Vector3(T _x, T _y, T _z) : Vector<T, 3>({_x, _y, _z}) {}
        Vector3<T> cross(Vector3<T>& other);
        Vector3(const Vector<T, 3>& other) : Vector<T, 3>(other) {}
        Vector3<T>& operator=(const Vector3<T>& other);
};

template<typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& other) {
    if (this != &other) {
        Vector<T, 3>::operator=(other);
    }

    return *this;
}

template<typename T>
Vector3<T> Vector3<T>::cross(Vector3<T>& other) {
    Vector3<T> result;
    result.x = this->y * other.z - this->z * other.y;
    result.y = this->z * other.x - this->x * other.z;
    result.z = this->x * other.y - this->y * other.x;
    return result;
}

template<typename T>
class Vector4 : public Vector<T, 4> {
    public:
        T& x = Vector<T, 4>::data[0];
        T& y = Vector<T, 4>::data[1];
        T& z = Vector<T, 4>::data[2];
        T& w = Vector<T, 4>::data[3];
        T& r = Vector<T, 4>::data[0];
        T& g = Vector<T, 4>::data[1];
        T& b = Vector<T, 4>::data[2];
        T& a = Vector<T, 4>::data[3];
        Vector4() : Vector<T, 4>() {}
        Vector4(T _x, T _y, T _z, T _w) : Vector<T, 4>({_x, _y, _z, _w}) {}
        Vector4(const Vector<T, 4>& other) : Vector<T, 4>(other) {}
        Vector4<T>& operator=(const Vector4<T>& other);
};

template<typename T>
Vector4<T>& Vector4<T>::operator=(const Vector4<T>& other) {
    if (this != &other) {
        Vector<T, 4>::operator=(other);
    }

    return *this;
}

template<typename T, size_t C>
class MatrixRow {
    public:
        MatrixRow();
        T& getColumn(size_t j);
        T& operator[](size_t j);
    private:
        T columns[C];
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
        template<size_t N> Matrix<T, R, N> multiply(Matrix<T, C, N>& other);
        Vector<T, C> multiply(Vector<T, C>& other);
        Matrix<T, C, R> transpose();
        Matrix<T, R, C> rowReduce();
        Matrix<T, R - 1, C - 1> minors(size_t i, size_t j);
        T minor(size_t i, size_t j);
        T cofactor(size_t i, size_t j);
        Matrix<T, R, C> cofactors();
        Matrix<T, R, C> adjugate();
        std::expected<Matrix<T, R, C>, Error> inverse();
        T determinant();
    private:
        MatrixRow<T, C> rows[R];
};

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
template<size_t N>
Matrix<T, R, N> Matrix<T, R, C>::multiply(Matrix<T, C, N>& other) {
    Matrix<T, R, N> result;

    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < N; ++j) {
            T sum = 0;
            for (size_t k = 0; k < C; ++k) {
                sum += this->rows[i][k] * other[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

template<typename T, size_t R, size_t C>
Matrix<T, C, R> Matrix<T, R, C>::transpose() {
    Matrix<T, C, R> transposed;

    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < C; ++j) {
            transposed[j][i] = this->rows[i][j];
        }
    }

    return transposed;
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
Matrix<T, R - 1, C - 1> Matrix<T, R, C>::minors(size_t i, size_t j) {
    static_assert(R == C, "Minors are only defined for square matrices");

    Matrix<T, R - 1, C - 1> minors;

    size_t minor_row = 0;
    for (size_t r = 0; r < R; ++r) {
        if (r == i) continue;
        size_t minor_column = 0;
        for (size_t c = 0; c < C; ++c) {
            if (c == j) continue;
            minors[minor_row][minor_column] = this->rows[r][c];
            ++minor_column;
        }
        ++minor_row;
    }

    return minors;
}

template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::minor(size_t i, size_t j) {
    static_assert(R == C, "Minor is only defined for square matrices");

    return this->minors(i, j).determinant();
}

template<typename T, size_t R, size_t C>
T Matrix<T, R, C>::cofactor(size_t i, size_t j) {
    static_assert(R == C, "Cofactor is only defined for square matrices");

    T minor = this->minor(i, j);
    T cofactor = (i + j) % 2 == 0 ? minor : minor * -1;
    return cofactor;
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::cofactors() {
    static_assert(R == C, "Cofactors are only defined for square matrices");

    Matrix<T, R, C> result;
    for (size_t i = 0; i < R; ++i) {
        for (size_t j = 0; j < C; ++j) {
            result[i][j] = this->cofactor(i, j);
        }
    }

    return result;
}

template<typename T, size_t R, size_t C>
Matrix<T, R, C> Matrix<T, R, C>::adjugate() {
    static_assert(R == C, "Adjugate is only defined for square matrices");

    return this->cofactors().transpose();
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

    Matrix<T, R, C> reduction = *this;
    float determinant = 1.0f;

    size_t lead = 0;
    for (size_t r = 0; r < R; ++r) {
        if (C <= lead)
            break;

        size_t i = r;
        while (reduction[i][lead] == 0) {
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
            std::swap(reduction[i], reduction[r]);
            determinant *= -1.0f;
        }

        T val = reduction[r][lead];
        if (val != 0) {
            for (size_t j = 0; j < C; ++j) {
                reduction[r][j] /= val;
            }
            determinant *= val;
        }

        for (size_t k = 0; k < R; ++k) {
            if (k != r) {
                T factor = reduction[k][lead];
                for (size_t j = 0; j < C; ++j) {
                    reduction[k][j] -= factor * reduction[r][j];
                }
            }
        }

        ++lead;
    }

    for (size_t i = 0; i < R; ++i) {
        determinant *= reduction[i][i];
    }

    return determinant;
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;
typedef Matrix<float, 3, 3> Matrix3x3f;
typedef Matrix<float, 4, 4> Matrix4x4f;

} // namespace apparition

#endif // APPARITION_MATH_HH