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

struct Vertex {
    Vector4f position;
    Vector4f color;
};

struct Primitive {};

struct Line : public Primitive {
    Line(Vertex _vertex_0, Vertex _vertex_1) : vertex_0(_vertex_0), vertex_1(_vertex_1) {}
    Vertex vertex_0;
    Vertex vertex_1;
};

struct Fragment {
    float depth = std::numeric_limits<float>::min();
    float t;
    Primitive* primitive = nullptr;
};

template<typename T>
class BaseBuffer2D {
    public:
        BaseBuffer2D(Vector2u dimensions);
        ~BaseBuffer2D();
        Vector2u getDimensions();
        T* getData();
        T& get(Vector2u position);
        void set(Vector2u position, T value);
    protected:
        size_t getIndex(Vector2u position);
        Vector2u dimensions;
        T* data;
};

template<typename T>
BaseBuffer2D<T>::BaseBuffer2D(Vector2u dimensions) {
    this->dimensions = dimensions;
    this->data = new T[dimensions.x * dimensions.y];
}

template<typename T>
BaseBuffer2D<T>::~BaseBuffer2D() {
    delete[] this->data;
}

template<typename T>
Vector2u BaseBuffer2D<T>::getDimensions() {
    return this->dimensions;
}

template<typename T>
T* BaseBuffer2D<T>::getData() {
    return this->data;
}

template<typename T>
T& BaseBuffer2D<T>::get(Vector2u position) {
    if (position.x < 0 || position.x >= this->dimensions.x || position.y < 0 || position.y >= this->dimensions.y) {
        throw std::out_of_range("'position' is out of range");
    }

    return this->data[this->getIndex(position)];
}

template<typename T>
void BaseBuffer2D<T>::set(Vector2u position, T value) {
    if (position.x < 0 || position.x >= this->dimensions.x || position.y < 0 || position.y >= this->dimensions.y) {
        throw std::out_of_range("'position' is out of range");
    }

    this->data[this->getIndex(position)] = value;
}

template<typename T>
size_t BaseBuffer2D<T>::getIndex(Vector2u position) {
    return (position.y * this->dimensions.x) + position.x;
}

class ColorBuffer : public BaseBuffer2D<Vector4f> {
    public:
        ColorBuffer(Vector2u dimensions) : BaseBuffer2D(dimensions) {}
};

class DepthBuffer : public BaseBuffer2D<Fragment> {
    public:
        DepthBuffer(Vector2u dimensions) : BaseBuffer2D(dimensions) {}
};

class FrameBuffer {
    public:
        FrameBuffer(Vector2u dimensions);
        ~FrameBuffer();
        Vector2u getDimensions();
        ColorBuffer* getColorBuffer();
        DepthBuffer* getDepthBuffer();
    private:
        Vector2u dimensions;
        ColorBuffer* color_buffer;
        DepthBuffer* depth_buffer;
};

class Shader;

class Renderer {
    public:
        Renderer();
        void bindFrameBuffer(FrameBuffer* to_bind);
        void bindVertexBuffer(std::vector<Vertex>* to_bind);
        void bindIndexBuffer(std::vector<size_t>* to_bind);
        void bindShader(Shader* to_bind);
        void drawLines();
    private:
        FrameBuffer* frame_buffer;
        std::vector<Vertex>* vertex_buffer;
        std::vector<size_t>* index_buffer;
        Shader* shader;
        void runVertexShader(Vertex& in_vertex);
        void runFragmentShader(Vector2u in_fragment_position, Fragment in_fragment);
};

} // namespace apparition

#endif // APPARITION_RENDERER_HH