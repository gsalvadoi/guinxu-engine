#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>

#include "types.h"
#include "textures.h"
#include "draw2d.h"
#include "colors.h"
#include "transforms.h"

void DrawProjectedTriangleColor(Renderer& renderer, const VertexProjected v1, const VertexProjected v2, const VertexProjected v3);
void DrawProjectedTriangleTexture(Renderer& renderer, const VertexProjected v1, const VertexProjected v2, const VertexProjected v3, UVs uv1, UVs uv2, UVs uv3, Texture& texture, Vec3 norm0, Vec3 norm1, Vec3 norm2);

float Interpolate(float val1, float val2, float val3, float z1, float z2, float z3, float w1, float w2, float w3, float invZ1, float invZ2, float invZ3, float invDenominatorInterpolation);
float PerspectiveCorrectInterpolation(float val1, float val2, float val3, float z1, float z2, float z3, float w1, float w2, float w3, float invZ1, float invZ2, float invZ3, float invDenominatorInterpolation);
float AffineInterpolation(float val1, float val2, float val3, float w1, float w2, float w3);