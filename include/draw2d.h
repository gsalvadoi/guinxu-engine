#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>

#include "types.h"
#include "textures.h"
#include "colors.h"

void DrawPixel(Renderer& renderer, int x, int y, uint32_t color);
void DrawLine(Renderer& renderer, int x1, int y1, int x2, int y2, uint32_t color);
void DrawTriangleWireframe(Renderer& renderer, Vec2 v1, Vec2 v2, Vec2 v3, uint32_t color);
void DrawTriangleBarycentric(Renderer& renderer, Vec2 pos1, Vec2 pos2, Vec2 pos3, uint32_t col1, uint32_t col2, uint32_t col3);