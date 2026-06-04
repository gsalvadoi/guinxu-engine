#include "draw3d.h"

using namespace std;

//Draws a triangle that has already been rasterized with the selected colors:
void DrawProjectedTriangleColor(Renderer& renderer, const VertexProjected v1, const VertexProjected v2, const VertexProjected v3)
{
    if (v1.z <= 0 || v2.z <= 0 || v3.z <= 0) { return; }

    Vec2 pos1 = v1.position;
    Vec2 pos2 = v2.position;
    Vec2 pos3 = v3.position;
    uint32_t col1 = PackColor(v1.color);
    uint32_t col2 = PackColor(v2.color);
    uint32_t col3 = PackColor(v3.color);

    //Boundary box:
    int minX = min({pos1.x, pos2.x, pos3.x});
    int maxX = max({pos1.x, pos2.x, pos3.x});
    int minY = min({pos1.y, pos2.y, pos3.y});
    int maxY = max({pos1.y, pos2.y, pos3.y});

    minX = max(minX, 0);
    minY = max(minY, 0);
    maxX = min(maxX, renderer.width - 1);
    maxY = min(maxY, renderer.height - 1);

    //avoid dividing by 0:
    float denominator = (pos2.y - pos3.y) * (pos1.x - pos3.x) + (pos3.x - pos2.x) * (pos1.y - pos3.y);
    if (fabs(denominator) < 0.00001f) { return; }

    //If the (x,y) point is inside the triangle, draw the pixel:
    for(int y = minY; y <= maxY; y++)
    {
        for(int x = minX; x <= maxX; x++)
        {
            float w1 = ((pos2.y - pos3.y) * (x - pos3.x) + (pos3.x - pos2.x) * (y - pos3.y)) / denominator;
            float w2 = ((pos3.y - pos1.y) * (x - pos3.x) + (pos1.x - pos3.x) * (y - pos3.y)) / denominator;
            float w3 = 1.0 - w1 - w2;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                float r = w1 * UnpackColor(col1).r + w2 * UnpackColor(col2).r + w3 * UnpackColor(col3).r;
                float g = w1 * UnpackColor(col1).g + w2 * UnpackColor(col2).g + w3 * UnpackColor(col3).g;
                float b = w1 * UnpackColor(col1).b + w2 * UnpackColor(col2).b + w3 * UnpackColor(col3).b;

                float z = w1 * v1.z + w2 * v2.z + w3 * v3.z;

                if (x >= 0 && x < renderer.width && y >= 0 && y < renderer.height)
                {
                    int index = x + y * renderer.width;
                    if (z < renderer.zBuffer[index])
                    {
                        renderer.zBuffer[index] = z;
                        DrawPixel(renderer, x, y, PackColor({255, (uint8_t)r, (uint8_t)g, (uint8_t)b}));
                    }
                }
            }
        }
    }
}

//Draws a triangle that has already been rasterized with a texture:
void DrawProjectedTriangleTexture(Renderer& renderer, const VertexProjected v1, const VertexProjected v2, const VertexProjected v3, UVs uv1, UVs uv2, UVs uv3, Texture& texture, Vec3 norm0, Vec3 norm1, Vec3 norm2)
{
    if (v1.z <= 0 || v2.z <= 0 || v3.z <= 0) { return; }

    Vec2 pos1 = v1.position;
    Vec2 pos2 = v2.position;
    Vec2 pos3 = v3.position;

    //Boundary box:
    int minX = min({pos1.x, pos2.x, pos3.x});
    int maxX = max({pos1.x, pos2.x, pos3.x});
    int minY = min({pos1.y, pos2.y, pos3.y});
    int maxY = max({pos1.y, pos2.y, pos3.y});

    minX = max(minX, 0);
    minY = max(minY, 0);
    maxX = min(maxX, renderer.width - 1);
    maxY = min(maxY, renderer.height - 1);

    //avoid dividing by 0:
    float denominator = (pos2.y - pos3.y) * (pos1.x - pos3.x) + (pos3.x - pos2.x) * (pos1.y - pos3.y);
    if (fabs(denominator) < 0.00001f) { return; }

    //Calculating recurrent divisions outside the loop:
    float invZ1 = 1.0 / v1.z;
    float invZ2 = 1.0 / v2.z;
    float invZ3 = 1.0 / v3.z;

    //If the (x,y) point is inside the triangle, draw the pixel:
    for(int y = minY; y <= maxY; y++)
    {
        for(int x = minX; x <= maxX; x++)
        {
            float w1 = ((pos2.y - pos3.y) * (x - pos3.x) + (pos3.x - pos2.x) * (y - pos3.y)) / denominator;
            float w2 = ((pos3.y - pos1.y) * (x - pos3.x) + (pos1.x - pos3.x) * (y - pos3.y)) / denominator;
            float w3 = 1.0 - w1 - w2;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                float denominatorInterpolation = w1 * invZ1 + w2 * invZ2 + w3 * invZ3;
                float invDenominatorInterpolation = 1.0f / denominatorInterpolation;

                float z = Interpolate(v1.z, v2.z, v3.z, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);

                if (x >= 0 && x < renderer.width && y >= 0 && y < renderer.height)
                {
                    int index = x + y * renderer.width;
                    if (z < renderer.zBuffer[index])
                    {
                        //Only calculate UVs and Normals when the pixel can be drawn:
                        float u = Interpolate(uv1.u, uv2.u, uv3.u, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);
                        float v = Interpolate(uv1.v, uv2.v, uv3.v, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);

                        float normX = Interpolate(norm0.x, norm1.x, norm2.x, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);
                        float normY = Interpolate(norm0.y, norm1.y, norm2.y, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);
                        float normZ = Interpolate(norm0.z, norm1.z, norm2.z, v1.z, v2.z, v3.z, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);
                        Vec3 norm = Normalize({normX, normY, normZ});

                        uint32_t colorToDraw = GetTexturePixelColor01(texture, u, v);
                        Color unpackedColor = UnpackColor(colorToDraw);

                        //Alpha cutout:
                        if (unpackedColor.a > 128)
                        {
                            Vec3 pixelPosition = {x, y, z};
                            Vec3 cameraPosition = renderer.camera.position - pixelPosition;
                            Color colorToDrawStruct =  ApplyShaders(renderer, unpackedColor, norm,  cameraPosition);

                            renderer.zBuffer[index] = z;
                            DrawPixel(renderer, x, y, PackColor(colorToDrawStruct));
                        }
                    }
                }
            }

        }
    }
}

//Interpolate between the vertices of a triangle:
float Interpolate(float val1, float val2, float val3, float z1, float z2, float z3, float w1, float w2, float w3, float invZ1, float invZ2, float invZ3, float invDenominatorInterpolation)
{
    //return AffineInterpolation(val1, val2, val3, w1, w2, w3);
    return PerspectiveCorrectInterpolation(val1, val2, val3, z1, z2, z3, w1, w2, w3, invZ1, invZ2, invZ3, invDenominatorInterpolation);
}

//Interpolate taking the depth into account:
float PerspectiveCorrectInterpolation(float val1, float val2, float val3, float z1, float z2, float z3, float w1, float w2, float w3, float invZ1, float invZ2, float invZ3, float invDenominatorInterpolation)
{
    float numerator = w1 * val1 * invZ1 + w2 * val2 * invZ2 + w3 * val3 * invZ3;
    return numerator * invDenominatorInterpolation;
}

//Interpolate using only affine operations, as in PlayStation 1:
float AffineInterpolation(float val1, float val2, float val3, float w1, float w2, float w3)
{
    float result = w1 * val1 + w2 * val2 + w3 * val3;
    return result;
}

