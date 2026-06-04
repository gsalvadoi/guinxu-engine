#include "vectors.h"

using namespace std;

//Returns the dot product of 2 vectors:
float DotProduct(Vec3 v1, Vec3 v2)
{
    float dotProduct = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return dotProduct;
}

//Return a normalized version of the input vector:
Vec3 Normalize(Vec3 v)
{
    Vec3 result = {0, 0, 0};
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0)
    {
        result.x = v.x / length;
        result.y = v.y / length;
        result.z = v.z / length;
    }
    return result;
}

//Add 2 Vec3 by adding their components respectively:
Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    Vec3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

//Substract 2 Vec3 by substracting their components respectively:
Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    Vec3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

//Multiply a Vec3 by a constant:
Vec3 operator*(const float f, const Vec3& v)
{
    Vec3 result;

    result.x = v.x * f;
    result.y = v.y * f;
    result.z = v.z * f;

    return result;
}

//Add 2 Vec2 elements by adding their components respectively:
Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
    Vec2 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;

    return result;
}

//Substract 2 Vec2 elements by substracting their components respectively:
Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
    Vec2 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;

    return result;
}