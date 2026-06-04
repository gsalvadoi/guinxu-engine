#pragma once

struct Vec2
{
    int x;
    int y;
};

struct Vec3
{
    float x;
    float y;
    float z;
};

struct UVs
{
    float u;
    float v;
};

struct Color
{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorFloat
{
    float a;
    float r;
    float g;
    float b;
};

struct Lights
{
    Vec3 lightDirection;
    Color lightColor;
    Color ambientColor;
};

struct Camera
{
    Vec3 position;
    Vec3 rotation;
};

struct Renderer
{
    int width;
    int height;
    uint32_t* pixels;
    float* zBuffer;
    Lights lights;
    Camera camera;
};

struct Vertex
{
    Vec2 position;
    Color color;
};

struct VertexProjected
{
    Vec2 position;
    Color color;
    float z;
};

struct Model
{
    Vec3* vertices;
    Color* colors;
    UVs* uv;
    Vec3* normals;

    int (*triangles)[3];
    int (*trianglesUV)[3];
    int (*trianglesNorm)[3];

    int vertexCount;
    int uvCount;
    int triangleCount;
    int normalsCount;
};

struct Texture
{
    int width;
    int height;
    uint32_t* pixels;
};

struct Text8x8Number
{
    uint32_t number[64];
};

struct Text8x8Font
{
    Text8x8Number numbers[10];
};