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
#include <fstream>

#include "renderer.hh"
#include "shader.hh"

using namespace apparition;

// Define the TGA header structure
#pragma pack(push, 1)
struct TGAHeader {
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t image_type;
    uint16_t color_map_origin;
    uint16_t color_map_length;
    uint8_t color_map_depth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t bits_per_pixel;
    uint8_t image_descriptor;
};
#pragma pack(pop)

void saveFrameBufferToTGA(const char* filename, ColorBuffer* colorBuffer) {
    if (!colorBuffer) {
        std::cerr << "Color buffer is nullptr." << std::endl;
        return;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write the TGA header
    TGAHeader header;
    std::memset(&header, 0, sizeof(TGAHeader));
    header.image_type = 2; // Uncompressed true-color image
    header.width = static_cast<uint16_t>(colorBuffer->getDimensions().x);
    header.height = static_cast<uint16_t>(colorBuffer->getDimensions().y);
    header.bits_per_pixel = 32; // 4 bytes per pixel (RGBA)
    file.write(reinterpret_cast<char*>(&header), sizeof(TGAHeader));

    // Write pixel data (RGBA) to the file
    const Vector2u dimensions = colorBuffer->getDimensions();
    const Vector4f* pixels = colorBuffer->getData();
    for (size_t y = 0; y < dimensions.y; ++y) {
        for (size_t x = 0; x < dimensions.x; ++x) {
            const Vector4f& pixel = pixels[y * dimensions.x + x];
            // TGA format stores pixel data in BGRA order
            const uint8_t rgba[4] = {
                static_cast<uint8_t>(pixel.b * 255), // Blue
                static_cast<uint8_t>(pixel.g * 255), // Green
                static_cast<uint8_t>(pixel.r * 255), // Red
                static_cast<uint8_t>(pixel.a * 255)  // Alpha
            };
            file.write(reinterpret_cast<const char*>(&rgba), sizeof(rgba));
        }
    }

    file.close();
    std::cout << "TGA file saved: " << filename << std::endl;
}

class MyShader : public Shader {
    public:
        void runVertex() override {
        }

        void runFragment() override {
            this->out_fragment_color = this->varying_vertex_color;
        }
};

int main() {
    Vector2u dimensions(32, 32);
    FrameBuffer frame_buffer(dimensions);
    std::vector<Vertex> vertex_buffer{
        Vertex(Vector4f(0.0f, 0.0f, 0.0f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f)),
        Vertex(Vector4f(0.5f, 1.0f, 0.0f, 0.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f)),
        Vertex(Vector4f(1.0f, 0.0f, 0.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f))
    };
    std::vector<size_t> index_buffer{
        0, 1, 1, 2, 0, 2
    };
    MyShader shader;

    Renderer renderer;
    renderer.bindFrameBuffer(&frame_buffer);
    renderer.bindVertexBuffer(&vertex_buffer);
    renderer.bindIndexBuffer(&index_buffer);
    renderer.bindShader(&shader);
    renderer.drawLines();

    saveFrameBufferToTGA("output.tga", frame_buffer.getColorBuffer());

    return 0;
}