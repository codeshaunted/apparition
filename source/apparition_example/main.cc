// codeshaunted - apparition_example
// include/apparition_example/main.cc
// contains entry point
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

#include <iostream>

#include "apparition/math.hh"

int main() {
    apparition::Matrix3x3f matrix{
        {-1.0f, 2.0f, -1.0f},
        {-2.0f, 0.0f, 1.0f},
        {1.0f, -1.0f, 0.0f}
    };

    auto inverse = *matrix.inverse();
    auto C = matrix.multiply(inverse);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << C[i][j] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << inverse.determinant() << std::endl;
    std::cout << matrix.determinant() << std::endl;

    return 0;
}