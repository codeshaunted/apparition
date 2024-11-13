// codeshaunted - apparition
// include/apparition/shader.hh
// contains shader declarations
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

#ifndef APPARITION_SHADER_HH
#define APPARITION_SHADER_HH

#include "renderer.hh"

namespace apparition {

class Shader {
    public:
        Vertex* vertex;
        Vector2u in_fragment_position;
        float in_fragment_depth;
        Vector4f varying_vertex_color;
        Vector4f out_fragment_color;
        Shader() = default;
        virtual void runVertex() {}
        virtual void runFragment() {}
};

} // namespace apparition

#endif // APPARITION_SHADER_HH