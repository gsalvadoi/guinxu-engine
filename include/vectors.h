#pragma once
#include <stdint.h>
#include <cmath>
#include <algorithm>

#include "types.h"

float DotProduct(Vec3 v1, Vec3 v2);
Vec3 Normalize(Vec3 v);

Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const float f, const Vec3& v);

Vec2 operator+(const Vec2& v1, const Vec2& v2);
Vec2 operator-(const Vec2& v1, const Vec2& v2);