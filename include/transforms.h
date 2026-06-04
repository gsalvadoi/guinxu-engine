#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>

#include "types.h"
#include "textures.h"
#include "vectors.h"

Vec3 RotateY(Vec3 v, float angle);
void RotateVerticesY(Vec3* input, Vec3* output, int count, float angle);
Vec3 Translate(const Vec3& v, const Vec3& offset);
void TranslateVertices(Vec3* input, Vec3* output, int count, Vec3 offset);
Vec3 Scale(const Vec3& v, const Vec3& scale);
void ScaleVertices(Vec3* input, Vec3* output, int count, Vec3 scale);

VertexProjected Project(Renderer& renderer, Vec3 v);
void ProjectVertices(Renderer& renderer, Vec3* input, VertexProjected* output, int count);
void CameraTransform(Camera camera, Vec3* input, Vec3* output, int nElements);