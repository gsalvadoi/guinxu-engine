#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>

#include "types.h"
#include "draw3d.h"
#include "textures.h"
#include "transforms.h"
#include "vectors.h"

using namespace std;

Model LoadObjModel(const string& filename);
void SetModelTransforms(Renderer& renderer, Model& model, Vec3 position, Vec3 eulerAngles, Vec3 scale, VertexProjected* verticesProjected);

void DrawModelColor(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale, Model& model, Color color);
void DrawModelTexture(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale, Model& model, Texture& texture);
void DrawCube(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale);
void DrawCubeOld(Renderer& rendProps, Vec3 cubePos, float angle, Vec3 scale);