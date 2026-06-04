#include "draw2d.h"

using namespace std;

//Draw a single pixel on (x,y) with the specified 0xAARRGGBB color:
void DrawPixel(Renderer& renderer, int x, int y, uint32_t color)
{
    if (x < 0 || x >= renderer.width || y <0 || y >= renderer.height) { return; }
    renderer.pixels[x + (renderer.height - y) * renderer.width] = color;
}

//Draw a line made of consecutive pixels between (x1, y1) and (x2, y2) with the specified 0xAARRGGBB color:
void DrawLine(Renderer& renderer, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1; //x distance
    int dy = y2 - y1; //y distance
    int steps = max(abs(dx), abs(dy)); //find the longest distance: x or y
    float xIncrement = 1.0 * dx / steps;
    float yIncrement = 1.0 * dy / steps;

    float x = x1;
    float y = y1;

    for(int i = 0; i <= steps; i++)
    {
        DrawPixel(renderer, (int)x, (int)y, color);
        x += xIncrement;
        y += yIncrement;
    }
}

//Draw a 2D triangle wireframe:
void DrawTriangleWireframe(Renderer& renderer, Vec2 v1, Vec2 v2, Vec2 v3, uint32_t color)
{
    DrawLine(renderer, v1.x, v1.y, v2.x, v2.y, color);
    DrawLine(renderer, v2.x, v2.y, v3.x, v3.y, color);
    DrawLine(renderer, v3.x, v3.y, v1.x, v1.y, color);
}

//Draw a 2D barycentric triangle (x1, y1) (x2, y2) (x3, y3) with the specified 0xAARRGGBB colors:
void DrawTriangleBarycentric(Renderer& renderer, Vec2 pos1, Vec2 pos2, Vec2 pos3, uint32_t col1, uint32_t col2, uint32_t col3)
{
    //Boundary box:
    int minX = min({pos1.x, pos2.x, pos3.x});
    int maxX = max({pos1.x, pos2.x, pos3.x});
    int minY = min({pos1.y, pos2.y, pos3.y});
    int maxY = max({pos1.y, pos2.y, pos3.y});

    minX = max(minX, 0);
    minY = max(minY, 0);
    maxX = min(maxX, renderer.width - 1);
    maxY = min(maxY, renderer.height - 1);

    float denominator = (pos2.y - pos3.y) * (pos1.x - pos3.x) + (pos3.x - pos2.x) * (pos1.y - pos3.y);
    if (fabs(denominator) < 0.00001f) { return; }

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

                if (x >= 0 && x < renderer.width && y >= 0 && y < renderer.height)
                {
                    DrawPixel(renderer, x, y, PackColor({255, (uint8_t)r, (uint8_t)g, (uint8_t)b}));
                }
            }
        }
    }
}
