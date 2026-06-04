#include "transforms.h"

using namespace std;

//Rotate a Vec3 around the Y axis:
Vec3 RotateY(Vec3 v, float angle)
{
    Vec3 result;
    result.x = v.x * cos(angle) - v.z * sin(angle);
    result.z = v.x * sin(angle) + v.z * cos(angle);
    result.y = v.y;

    return result;
}

//Rotate all the vertices from "input" around the Y axis:
void RotateVerticesY(Vec3* input, Vec3* output, int count, float angle)
{
    for(int i = 0; i < count; i++)
    {
        output[i] = RotateY(input[i], angle);
    }
}

//Translate a Vec3 by an offset:
Vec3 Translate(const Vec3& v, const Vec3& offset)
{
    return {v.x + offset.x, v.y + offset.y, v.z + offset.z};
}

//Translate all the vertices from "input" by an offset:
void TranslateVertices(Vec3* input, Vec3* output, int count, Vec3 offset)
{
    for(int i = 0; i < count; i++)
    {
        output[i] = Translate(input[i], offset);
    }
}

//Scale a Vec3:
Vec3 Scale(const Vec3& v, const Vec3& scale)
{
    return {v.x * scale.x, v.y * scale.y, v.z * scale.z};
}

//Scale all the vertices from "input" by a scale:
void ScaleVertices(Vec3* input, Vec3* output, int count, Vec3 scale)
{
    for(int i = 0; i < count; i++)
    {
        output[i] = Scale(input[i], scale);
    }
}

//Project a Vec3 vertex into the screen:
VertexProjected Project(Renderer& renderer, Vec3 v)
{
    float zNear = 0.1;
    float zFar = 30.0;
    int fov = 300;
    VertexProjected result;
    result.position.x = (v.x / v.z) * fov + renderer.width / 2;
    result.position.y = (v.y / v.z) * fov + renderer.height / 2;
    result.z = (v.z - zNear) / (zFar - zNear);
    return result;
}

//Project a Vec3 vertex into the screen:
void ProjectVertices(Renderer& renderer, Vec3* input, VertexProjected* output, int count)
{
    for(int i = 0; i < count; i++)
    {
        output[i] = Project(renderer, input[i]);
    }
}

//Translate and rotate the vertices of an object in relation to the camera position and Y-rotation:
void CameraTransform(Camera camera, Vec3* input, Vec3* output, int vertices)
{
    for(int i = 0; i < vertices; i++)
    {
        Vec3 v = input[i] - camera.position;
        v = RotateY(v, -camera.rotation.y);
        output[i] = v;
    }
}