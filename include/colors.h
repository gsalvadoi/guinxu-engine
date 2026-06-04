#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>

#include "types.h"
#include "textures.h"
#include "vectors.h"

Color UnpackColor(uint32_t color);
ColorFloat UnpackColorFloat(uint32_t color);
uint32_t PackColor(const Color& c);
uint32_t PackColorFloat(const ColorFloat& cf);

uint8_t floatToUint8(float value);
uint32_t expandUint8ToUint32(uint8_t value);

ColorFloat LambertFormula(Color lightColor, Vec3 normal, Vec3 lightDir);
ColorFloat PhongFormula(Vec3 normal, Vec3 lightDir, Vec3 cameraDir);
Color ApplyShaders(Renderer& renderer, Color albedo, Vec3 normal, Vec3 cameraDir);

ColorFloat ClampColor(ColorFloat color);
ColorFloat operator+(const ColorFloat& c1, const ColorFloat& c2);
ColorFloat operator-(const ColorFloat& c1, const ColorFloat& c2);
ColorFloat operator*(const ColorFloat& c1, const ColorFloat& c2);

ColorFloat ColorToColorFloat(const Color& color);
Color ColorFloatToColor(const ColorFloat& color);