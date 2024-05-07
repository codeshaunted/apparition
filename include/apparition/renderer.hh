// codeshaunted - apparition
// include/apparition/renderer.hh
// contains renderer declarations
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

#ifndef APPARITION_RENDERER_HH
#define APPARITION_RENDERER_HH

#include <vector>

#include "math.hh"

namespace apparition {

class Vertex {
    Vector3f position;
    Vector4f color;
};

template<typename T>
class BaseBuffer2D {
    public:
        BaseBuffer2D(Vector2i dimensions);
        ~BaseBuffer2D();
        T get(Vector2i position);
        void set(Vector2i position, T value);
    protected:
        size_t getIndex(Vector2i position);
        Vector2i dimensions;
        T* data;
};

class ColorBuffer : public BaseBuffer2D<Vector4f> {
    public:
        ColorBuffer(Vector2i dimensions) : BaseBuffer2D(dimensions) {}
};

class DepthBuffer : public BaseBuffer2D<float> {
    public:
        DepthBuffer(Vector2i dimensions);
};

class FrameBuffer {
    public:
        FrameBuffer(Vector2i dimensions);
        ~FrameBuffer();
        ColorBuffer* getColorBuffer();
        DepthBuffer* getDepthBuffer();
    private:
        Vector2i dimensions;
        ColorBuffer* color_buffer;
        DepthBuffer* depth_buffer;
};

class Renderer {
    public:
        Renderer();
        void bindFrameBuffer(FrameBuffer* to_bind);
        void bindVertexBuffer(std::vector<Vertex>* to_bind);
        void bindIndexBuffer(std::vector<size_t>* to_bind);
    private:
        FrameBuffer* frame_buffer;
        std::vector<Vertex>* vertex_buffer;
        std::vector<size_t>* index_buffer;
};

} // namespace apparition

#endif // APPARITION_RENDERER_HH