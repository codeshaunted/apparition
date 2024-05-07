// codeshaunted - apparition
// source/apparition/renderer.cc
// contains renderer definitions
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

#include "renderer.hh"

namespace apparition {

template<typename T>
BaseBuffer2D<T>::BaseBuffer2D(Vector2i dimensions) {
    this->dimensions = dimensions;

    this->data = (T*)std::malloc(sizeof(T) * this->dimensions.x * this->dimensions.y);

    for (size_t i = 0; i < this->dimensions.x * this->dimensions.y; ++i) {
        this->data[i] = T();
    }
}

template<typename T>
BaseBuffer2D<T>::~BaseBuffer2D() {
    std::free(this->data);
}

template<typename T>
T BaseBuffer2D<T>::get(Vector2i position) {
    if (position.x < 0 || position.x >= this->dimensions.x || position.y < 0 || position.y >= this->dimensions.y) {
        throw std::out_of_range("'position' is out of range");
    }

    return this->data[this->getIndex(position)];
}

template<typename T>
void BaseBuffer2D<T>::set(Vector2i position, T value) {
    if (position.x < 0 || position.x >= this->dimensions.x || position.y < 0 || position.y >= this->dimensions.y) {
        throw std::out_of_range("'position' is out of range");
    }

    this->data[this->getIndex(position)] = value;
}

template<typename T>
size_t BaseBuffer2D<T>::getIndex(Vector2i position) {
    return (position.y * this->dimensions.x) + position.x;
}

DepthBuffer::DepthBuffer(Vector2i dimensions) : BaseBuffer2D(dimensions) {
    for (size_t i = 0; i < this->dimensions.x * this->dimensions.y; ++i) {
        this->data[i] = std::numeric_limits<float>::min();
    }
}

FrameBuffer::FrameBuffer(Vector2i dimensions) {
    this->dimensions = dimensions;

    this->color_buffer = new ColorBuffer(dimensions);
    this->depth_buffer = new DepthBuffer(dimensions);
}

FrameBuffer::~FrameBuffer() {
    delete this->color_buffer;
    delete this->depth_buffer;
}

ColorBuffer* FrameBuffer::getColorBuffer() {
    return this->color_buffer;
}

DepthBuffer* FrameBuffer::getDepthBuffer() {
    return this->depth_buffer;
}

Renderer::Renderer() {}

void Renderer::bindFrameBuffer(FrameBuffer* to_bind) {
    if (!to_bind) {
        throw std::invalid_argument("'to_bind' cannot be nullptr");
    }

    this->frame_buffer = to_bind;
}

void Renderer::bindVertexBuffer(std::vector<Vertex>* to_bind) {
    if (!to_bind) {
        throw std::invalid_argument("'to_bind' cannot be nullptr");
    }

    this->vertex_buffer = to_bind;
}

void Renderer::bindIndexBuffer(std::vector<size_t>* to_bind) {
    if (!to_bind) {
        throw std::invalid_argument("'to_bind' cannot be nullptr");
    }

    this->index_buffer = to_bind;
}

} // namespace apparition