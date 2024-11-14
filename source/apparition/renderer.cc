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
#include "shader.hh"

namespace apparition {

FrameBuffer::FrameBuffer(Vector2u dimensions) {
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

Vector2u FrameBuffer::getDimensions() {
    return this->dimensions;
}

Renderer::Renderer() {
    this->frame_buffer = nullptr;
    this->vertex_buffer = nullptr;
    this->index_buffer = nullptr;
    this->shader = nullptr;
}

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

void Renderer::bindShader(Shader* to_bind) {
    if (!to_bind) {
        throw std::invalid_argument("'to_bind' cannot be nullptr");
    }

    this->shader = to_bind;
}

void Renderer::drawLines() {
    if (!this->frame_buffer) {
        throw std::logic_error("No frame buffer bound");
    }
    if (!this->vertex_buffer) {
        throw std::logic_error("No vertex buffer bound");
    }
    if (!this->index_buffer) {
        throw std::logic_error("No index buffer bound");
    }
    if (!this->shader) {
        throw std::logic_error("No shader bound");
    }

    if (this->index_buffer->size() % 2 != 0) {
        throw std::invalid_argument("Index buffer size must be divisible by 2");
    }

    Vector2u dimensions = this->frame_buffer->getDimensions();

    std::vector<Line> lines;
    for (size_t i = 0; i < this->index_buffer->size(); i += 2) {
        size_t index_0 = this->index_buffer->at(i);
        size_t index_1 = this->index_buffer->at(i + 1);

        if (index_0 < 0 || index_0 >= this->vertex_buffer->size() || index_1 < 0 || index_1 >= this->vertex_buffer->size()) {
            throw std::out_of_range("Index out of range");
        }

        Vertex vertex_0 = this->vertex_buffer->at(index_0);
        Vertex vertex_1 = this->vertex_buffer->at(index_1);

        this->runVertexShader(vertex_0);
        this->runVertexShader(vertex_1);

        Line line(vertex_0, vertex_1);

        lines.push_back(line);
    }

    for (Line& line : lines) {
        // draw line using bresenham's algorithm
        // based on pseudocode stolen from wikipedia

        int original_x0 = line.vertex_0.position.x * (dimensions.x - 1);
        int original_x1 = line.vertex_1.position.x * (dimensions.x - 1);
        int original_y0 = line.vertex_0.position.y * (dimensions.y - 1);
        int original_y1 = line.vertex_1.position.y * (dimensions.y - 1);

        int x0 = original_x0;
        int x1 = original_x1;
        int y0 = original_y0;
        int y1 = original_y1;

        int dx = std::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int error = dx + dy;

        float total_distance = std::sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));

        for (;;) {
            Fragment& fragment = this->frame_buffer->getDepthBuffer()->get(Vector2u(x0, y0));
            fragment.primitive = static_cast<Primitive*>(&line);

            float current_distance = std::sqrt((x0 - original_x0) * (x0 - original_x0) + (y0 - original_y0) * (y0 - original_y0));
            fragment.t = current_distance / total_distance;

            if (x0 == x1 && y0 == y1) {
                break;
            }

            int e2 = 2 * error;
            if (e2 >= dy) {
                error += dy;
                x0 += sx;
            }
            
            if (e2 <= dx) {
                error += dx;
                y0 += sy;
            }
        }
    }

    for (size_t i = 0; i < dimensions.x; ++i) {
        for (size_t j = 0; j < dimensions.y; ++j) {
            Vector2u fragment_position = Vector2u(i, j);
            Fragment& fragment = this->frame_buffer->getDepthBuffer()->get(fragment_position);

            this->runFragmentShader(fragment_position, fragment);
            this->frame_buffer->getColorBuffer()->set(fragment_position, this->shader->out_fragment_color);
        }
    }
}

void Renderer::drawTris() {
    if (!this->frame_buffer) {
        throw std::logic_error("No frame buffer bound");
    }
    if (!this->vertex_buffer) {
        throw std::logic_error("No vertex buffer bound");
    }
    if (!this->index_buffer) {
        throw std::logic_error("No index buffer bound");
    }
    if (!this->shader) {
        throw std::logic_error("No shader bound");
    }

    if (this->index_buffer->size() % 3 != 0) {
        throw std::invalid_argument("Index buffer size must be divisible by 3");
    }

    Vector2u dimensions = this->frame_buffer->getDimensions();

    std::vector<Tri> tris;
    for (size_t i = 0; i < this->index_buffer->size(); i += 3) {
        size_t index_0 = this->index_buffer->at(i);
        size_t index_1 = this->index_buffer->at(i + 1);
        size_t index_2 = this->index_buffer->at(i + 2);

        if (index_0 < 0 || index_0 >= this->vertex_buffer->size() || index_1 < 0 || index_1 >= this->vertex_buffer->size() || index_2 < 0 || index_2 >= this->vertex_buffer->size()) {
            throw std::out_of_range("Index out of range");
        }

        Vertex vertex_0 = this->vertex_buffer->at(index_0);
        Vertex vertex_1 = this->vertex_buffer->at(index_1);
        Vertex vertex_2 = this->vertex_buffer->at(index_2);

        this->runVertexShader(vertex_0);
        this->runVertexShader(vertex_1);
        this->runVertexShader(vertex_2);

        Tri tri(vertex_0, vertex_1, vertex_2);

        tris.push_back(tri);
    }

    for (Tri& tri : tris) {
        // draw tri using barycentric algorithm
        
        float x0 = tri.vertex_0.position.x * (dimensions.x - 1);
        float x1 = tri.vertex_1.position.x * (dimensions.x - 1);
        float x2 = tri.vertex_2.position.x * (dimensions.x - 1);
        float y0 = tri.vertex_0.position.y * (dimensions.y - 1);
        float y1 = tri.vertex_1.position.y * (dimensions.y - 1);
        float y2 = tri.vertex_2.position.y * (dimensions.y - 1);

        float denominator = ((y1 - y2) * (x0 - x2)) + ((x2 - x1) * (y0 - y2));
        
        for (uint32_t x = 0; x < dimensions.x; ++x) {
            for (uint32_t y = 0; y < dimensions.y; ++y) {
                float b0 = (((y1 - y2) * (x - x2)) + ((x2 - x1) * (y - y2))) / denominator;
                float b1 = (((y2 - y0) * (x - x2)) + ((x0 - x2) * (y - y2))) / denominator;
                float b2 = 1 - b0 - b1;

                if (b0 >= 0.0f && b0 <= 1.0f && b1 >= 0.0f && b1 <= 1.0f && b2 >= 0.0f && b2 <= 1.0f) {
                    Fragment& fragment = this->frame_buffer->getDepthBuffer()->get(Vector2u(x, y));
                    fragment.primitive = static_cast<Primitive*>(&tri);
                    fragment.b0 = b0;
                    fragment.b1 = b1;
                    fragment.b2 = b2;
                }
            }
        }
    }

    for (size_t i = 0; i < dimensions.x; ++i) {
        for (size_t j = 0; j < dimensions.y; ++j) {
            Vector2u fragment_position = Vector2u(i, j);
            Fragment& fragment = this->frame_buffer->getDepthBuffer()->get(fragment_position);

            this->runFragmentShader(fragment_position, fragment);
            this->frame_buffer->getColorBuffer()->set(fragment_position, this->shader->out_fragment_color);
        }
    }
}

void Renderer::runVertexShader(Vertex& in_vertex) {
    if (!this->shader) {
        throw std::logic_error("No shader bound");
    }

    this->shader->vertex = &in_vertex;
    this->shader->runVertex();
}

void Renderer::runFragmentShader(Vector2u in_fragment_position, Fragment in_fragment) {
    if (!this->shader) {
        throw std::logic_error("No shader bound");
    }

    this->shader->in_fragment_position = in_fragment_position;
    this->shader->in_fragment_depth = in_fragment.depth;
    this->shader->out_fragment_color = Vector4f();
    this->shader->varying_vertex_color = Vector4f();

    if (in_fragment.primitive) {
        if (in_fragment.primitive->type == PrimitiveType::LINE) {
            Line* line = static_cast<Line*>(in_fragment.primitive);
            this->shader->varying_vertex_color.r = std::lerp(line->vertex_0.color.r, line->vertex_1.color.r, in_fragment.t);
            this->shader->varying_vertex_color.g = std::lerp(line->vertex_0.color.g, line->vertex_1.color.g, in_fragment.t);
            this->shader->varying_vertex_color.b = std::lerp(line->vertex_0.color.b, line->vertex_1.color.b, in_fragment.t);
            this->shader->varying_vertex_color.a = std::lerp(line->vertex_0.color.a, line->vertex_1.color.a, in_fragment.t);
        } else if (in_fragment.primitive->type == PrimitiveType::TRI) {
            Tri* tri = static_cast<Tri*>(in_fragment.primitive);
            this->shader->varying_vertex_color.r = (tri->vertex_0.color.r * in_fragment.b0) + (tri->vertex_1.color.r * in_fragment.b1) + (tri->vertex_2.color.r * in_fragment.b2);
            this->shader->varying_vertex_color.g = (tri->vertex_0.color.g * in_fragment.b0) + (tri->vertex_1.color.g * in_fragment.b1) + (tri->vertex_2.color.g * in_fragment.b2);
            this->shader->varying_vertex_color.b = (tri->vertex_0.color.b * in_fragment.b0) + (tri->vertex_1.color.b * in_fragment.b1) + (tri->vertex_2.color.b * in_fragment.b2);
            this->shader->varying_vertex_color.a = (tri->vertex_0.color.a * in_fragment.b0) + (tri->vertex_1.color.a * in_fragment.b1) + (tri->vertex_2.color.a * in_fragment.b2);    
        }
    }

    this->shader->runFragment();
}

} // namespace apparition