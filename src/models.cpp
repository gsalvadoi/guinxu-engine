#include "models.h"

using namespace std;

//Loads a .OBJ 3D model and stores it into a Model struct variable:
Model LoadObjModel(const string& filename)
{
    //Init model structure:
    Model model;

    ifstream file(filename);
    if (!file.is_open())
    {
        printf("Can't open file: %s \n", filename.c_str());
        return model;
    }

    vector<Vec3> vertices;
    vector<UVs> uvs;
    vector<Vec3> normals;
    vector<array<int, 3>> triangles;
    vector<array<int, 3>> trianglesUV;
    vector<array<int, 3>> trianglesNorm;
    int nVertices = 0;
    int nUVs = 0;
    int nTriangles = 0;
    int nNormals = 0;

    //Read OBJ file lines:
    string line;
    while(getline(file, line))
    {
        //Vertices:
        if (line.substr(0, 2) == "v ")
        {
            istringstream iss(line);
            string prefix;
            Vec3 vertex;
            iss >> prefix >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
            nVertices++;
        }

        else //UVs:
        if (line.substr(0, 3) == "vt ")
        {
            istringstream iss(line);
            string prefix;
            UVs uv;
            iss >> prefix >> uv.u >> uv.v;
            uvs.push_back(uv);
            nUVs++;
        }

        else //Normals:
        if (line.substr(0, 3) == "vn ")
        {
            istringstream iss(line);
            string prefix;
            Vec3 normal;
            iss >> prefix >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
            nNormals++;
        }

        //Triangles (faces):
        else if (line.substr(0, 2) == "f ")
        {
            istringstream iss(line);

            string prefix;
            string vData[3];
            iss >> prefix >> vData[0] >> vData[1] >> vData[2];

            array<int, 3> triangleVertex;
            array<int, 3> triangleUVs;
            array<int, 3> triangleNormals;

            for(int i = 0; i < 3; i++)
            {
                istringstream vContent(vData[i]);
                char slash;
                if (!(vContent >> triangleVertex[i] >> slash >> triangleUVs[i] >> slash >> triangleNormals[i]))
                {
                    printf("Error parsing 3d model faces\n");
                    printf("Content = %s\n", vData[i].c_str());
                    exit(1);
                }

                triangleVertex[i]--;
                triangleUVs[i]--;
                triangleNormals[i]--;
            }

            triangles.push_back(triangleVertex);
            trianglesUV.push_back(triangleUVs);
            trianglesNorm.push_back(triangleNormals);
            nTriangles++;
        }
    }

    file.close();

    //Store everything inside the Model struct:
    model.vertices = new Vec3[nVertices];
    model.triangles = new int[nTriangles][3];
    model.trianglesUV = new int[nTriangles][3];
    model.trianglesNorm = new int[nTriangles][3];
    model.colors = new Color[nVertices];
    model.uv = new UVs[nUVs];
    model.normals = new Vec3[nNormals];

    for(int i = 0; i < nVertices; i++)
    {
        model.vertices[i] = vertices[i];
        model.colors[i] = {255, 255, 255, 0};
    }

    for(int i = 0; i < nUVs; i++)
    {
        model.uv[i] = uvs[i];
    }

    for(int i = 0; i < nNormals; i++)
    {
        model.normals[i] = normals[i];
    }

    for(int i = 0; i < nTriangles; i++)
    {
        model.triangles[i][0] = triangles[i][0];
        model.triangles[i][1] = triangles[i][1];
        model.triangles[i][2] = triangles[i][2];

        model.trianglesUV[i][0] = trianglesUV[i][0];
        model.trianglesUV[i][1] = trianglesUV[i][1];
        model.trianglesUV[i][2] = trianglesUV[i][2];

        model.trianglesNorm[i][0] = trianglesNorm[i][0];
        model.trianglesNorm[i][1] = trianglesNorm[i][1];
        model.trianglesNorm[i][2] = trianglesNorm[i][2];
    }

    model.vertexCount = nVertices;
    model.triangleCount = nTriangles;
    model.uvCount = nUVs;
    model.normalsCount = nNormals;

    return model;
}

//Applies all 3D transforms and projection in one function:
void SetModelTransforms(Renderer& renderer, Model& model, Vec3 position, Vec3 eulerAngles, Vec3 scale, VertexProjected* verticesProjected)
{
    //Set vertices new position after 3D transformations:
    Vec3 verticesTransformed[model.vertexCount];
    RotateVerticesY(model.vertices, verticesTransformed, model.vertexCount, eulerAngles.y);

    ScaleVertices(verticesTransformed, verticesTransformed, model.vertexCount, scale);
    TranslateVertices(verticesTransformed, verticesTransformed, model.vertexCount, position);

    //Transform again to match the camera:
    Vec3 verticesView[model.vertexCount];
    CameraTransform(renderer.camera, verticesTransformed, verticesView, model.vertexCount);

    //Project vertices:
    ProjectVertices(renderer, verticesView, verticesProjected, model.vertexCount);
}

//Draw 3D model using the same color for every pixel:
void DrawModelColor(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale, Model& model, Color color)
{
    //Set all vertex colors the same color:
    Color* colors = new Color[model.vertexCount];
    for(int i = 0; i < model.vertexCount; i++) { colors[i] = color; }

    //Apply transforms and projection:
    VertexProjected verticesProjected[model.vertexCount];
    SetModelTransforms(renderer, model, position, eulerAngles, scale, verticesProjected);

    //Draw triangles on projected vertices:
    for(int i = 0; i < model.triangleCount; i++)
    {
        int i0 = model.triangles[i][0];
        int i1 = model.triangles[i][1];
        int i2 = model.triangles[i][2];

        if (verticesProjected[i0].z > 0 && verticesProjected[i1].z > 0 && verticesProjected[i2].z > 0)
        {
            VertexProjected vt0; vt0.position = verticesProjected[i0].position; vt0.color = colors[i0]; vt0.z = verticesProjected[i0].z;
            VertexProjected vt1; vt1.position = verticesProjected[i1].position; vt1.color = colors[i1]; vt1.z = verticesProjected[i1].z;
            VertexProjected vt2; vt2.position = verticesProjected[i2].position; vt2.color = colors[i2]; vt2.z = verticesProjected[i2].z;
            DrawProjectedTriangleColor(renderer, vt0, vt1, vt2);
        }
    }
}

//Draw 3D model using a texture to set the color of each pixel:
void DrawModelTexture(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale, Model& model, Texture& texture)
{
    //Apply transforms and projection:
    VertexProjected verticesProjected[model.vertexCount];
    SetModelTransforms(renderer, model, position, eulerAngles, scale, verticesProjected);

    //Apply normals rotation:
    Vec3 normalsTransformed[model.normalsCount];
    RotateVerticesY(model.normals, normalsTransformed, model.normalsCount, eulerAngles.y);

    //Draw triangles on projected vertices:
    for(int i = 0; i < model.triangleCount; i++)
    {
        int i0 = model.triangles[i][0];
        int i1 = model.triangles[i][1];
        int i2 = model.triangles[i][2];

        int uvTri0 = model.trianglesUV[i][0];
        int uvTri1 = model.trianglesUV[i][1];
        int uvTri2 = model.trianglesUV[i][2];

        int normTri0 = model.trianglesNorm[i][0];
        int normTri1 = model.trianglesNorm[i][1];
        int normTri2 = model.trianglesNorm[i][2];

        UVs uv0 = model.uv[uvTri0];
        UVs uv1 = model.uv[uvTri1];
        UVs uv2 = model.uv[uvTri2];

        Vec3 norm0 = normalsTransformed[normTri0];
        Vec3 norm1 = normalsTransformed[normTri1];
        Vec3 norm2 = normalsTransformed[normTri2];

        //Backface culling:
        Vec2 a = verticesProjected[i1].position - verticesProjected[i0].position;
        Vec2 b = verticesProjected[i2].position - verticesProjected[i0].position;
        float crossProduct = a.x * b.y - a.y * b.x;

        if (verticesProjected[i0].z > 0 && verticesProjected[i1].z > 0 && verticesProjected[i2].z > 0 && crossProduct < 0)
        {
            VertexProjected vt0;
            VertexProjected vt1;
            VertexProjected vt2;

            vt0.position = verticesProjected[i0].position;
            vt0.color = model.colors[i0];
            vt0.z = verticesProjected[i0].z;
            
            vt1.position = verticesProjected[i1].position;
            vt1.color = model.colors[i1];
            vt1.z = verticesProjected[i1].z;
            
            vt2.position = verticesProjected[i2].position;
            vt2.color = model.colors[i2];
            vt2.z = verticesProjected[i2].z;

            DrawProjectedTriangleTexture(renderer, vt0, vt1, vt2, uv0, uv1, uv2, texture, norm0, norm1, norm2);
        }
    }
}

//Draw a hardcoded cube 3D model:
void DrawCube(Renderer& renderer, Vec3 position, Vec3 eulerAngles, Vec3 scale)
{
    Vec3 cubeVertices[8] =
    {
        {1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {1, -1, -1},
        {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}
    };

    int cubeTriangles[12][3] =
    {
        {0, 1, 3}, {3, 2, 0},
        {1, 5, 7}, {7, 3, 1},
        {0, 4, 5}, {5, 1, 0},
        {4, 6, 7}, {7, 5, 4},
        {0, 4, 6}, {6, 2, 0},
        {6, 2, 3}, {3, 7, 6}
    };

    Color cubeColors[8] =
    {
        {255, 255, 0, 0}, {255, 255, 0, 0}, {255, 255, 0, 0}, {255, 255, 0, 0},
        {255, 0, 255, 0}, {255, 0, 255, 0}, {255, 0, 255, 0}, {255, 0, 255, 0},
    };

    Model model;
    model.vertices = cubeVertices;
    model.colors = cubeColors;
    model.triangles = cubeTriangles;
    model.vertexCount = 8;
    model.uvCount = 0;
    model.triangleCount = 12;

    Color color =  {255, 255, 0, 0};
    DrawModelColor(renderer, position, eulerAngles, scale, model, color);
}

//Draw a wireframe version of a cube 3D model:
void DrawCubeOld(Renderer& rendProps, Vec3 cubePos, float angle, Vec3 scale)
{
    Vec3 cubeVertices[8] =
    {
        {1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {1, -1, -1}, {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}
    };

    for (int i = 0; i < 8; i++)
    {
        cubeVertices[i].x *= scale.x;
        cubeVertices[i].y *= scale.y;
        cubeVertices[i].z *= scale.z;
    }

    int cubeTriangles[12][3] =
    {
        {0, 1, 3}, {3, 2, 0},
        {1, 5, 7}, {7, 3, 1},
        {0, 4, 5}, {5, 1, 0},
        {4, 6, 7}, {7, 5, 4},
        {0, 4, 6}, {6, 2, 0},
        {6, 2, 3}, {3, 7, 6}
    };

    Color cubeColors[6] =
    {
        {255, 255, 0, 0}, {255, 0, 255, 0}, {255, 0, 0, 255}, {255, 255, 255, 0}, {255, 255, 0, 255}, {255, 0, 255, 255}
    };

    //Apply transforms and projection:
    Vec3 cubeVerticesTransformed[8];
    RotateVerticesY(cubeVertices, cubeVerticesTransformed, 8, angle);
    TranslateVertices(cubeVerticesTransformed, cubeVerticesTransformed, 8, cubePos);

    VertexProjected verticesProjected[8];
    ProjectVertices(rendProps, cubeVerticesTransformed, verticesProjected, 8);

    for(int i = 0; i < 12; i++)
    {
        int i0 = cubeTriangles[i][0];
        int i1 = cubeTriangles[i][1];
        int i2 = cubeTriangles[i][2];

        VertexProjected vt0; vt0.position = verticesProjected[i0].position; vt0.color = cubeColors[i / 2]; vt0.z = verticesProjected[i0].z;
        VertexProjected vt1; vt1.position = verticesProjected[i1].position; vt1.color = cubeColors[i / 2]; vt1.z = verticesProjected[i1].z;
        VertexProjected vt2; vt2.position = verticesProjected[i2].position; vt2.color = cubeColors[i / 2]; vt2.z = verticesProjected[i2].z;
        DrawProjectedTriangleColor(rendProps, vt0, vt1, vt2);
    }

    for(int i = 0; i < 12; i++)
    {
        int i0 = cubeTriangles[i][0];
        int i1 = cubeTriangles[i][1];
        int i2 = cubeTriangles[i][2];

        //DrawTriangleWireframe(rendProps, verticesProjected[i0].position, verticesProjected[i1].position, verticesProjected[i2].position, 0xFFFFFFFF);
    }
}
