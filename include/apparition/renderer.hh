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
class BaseBuffer {
    public:
        BaseBuffer(Vector2i dimensions);
        ~BaseBuffer();
    protected:
        size_t getIndex(Vector2i position);
        Vector2i dimensions;
        T* data;
};

class ColorBuffer : public BaseBuffer<Vector4f> {
    public:
        ColorBuffer(Vector2i dimensions) : BaseBuffer(dimensions) {}
};

class DepthBuffer : public BaseBuffer<float> {
    public:
        DepthBuffer(Vector2i dimensions);
};

class FrameBuffer {
    public:
        FrameBuffer(Vector2i dimensions);
        ~FrameBuffer();
    private:
        Vector2i dimensions;
        ColorBuffer* color_buffer;
        DepthBuffer* depth_buffer;
};

class VertexBuffer {
    
};

class IndexBuffer {

};

class Renderer {
    public:
        Renderer();
        void bindFrameBuffer(FrameBuffer* to_bind);
        void bindVertexBuffer(VertexBuffer* to_bind);
        void bindIndexBuffer(IndexBuffer* to_bind);
    private:
        FrameBuffer* frame_buffer;
        VertexBuffer* vertex_buffer;
        IndexBuffer* index_buffer;
};

} // namespace apparition

#endif // APPARITION_RENDERER_HH