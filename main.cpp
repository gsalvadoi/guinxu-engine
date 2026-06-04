#include <stdint.h>
#include <SDL2/SDL.h>
#include <string>

#include "types.h"
#include "models.h"
#include "textures.h"
#include "colors.h"
#include "draw2d.h"
#include "draw3d.h"
#include "transforms.h"
#include "vectors.h"

using namespace std;

//Rendering variables:
Renderer rendProps;
bool zBufferView = false;
uint32_t* zBufferViewPixels;
float deltaTime = 0;
uint32_t lastFrameTime = 0;

//Object propperties:
float angle = 0;
Vec3 cubePos = {0, 0, 8};
Vec3 cubeScale = {1, 1, 1};
Vec3 ballPos = {1, -1.5, 3};
Vec3 marioPos = {-0.07, -2.185, 2.105};

Model exampleBall;
Model marioModel;
Model checkCubeModel;
Model houseModel;

Texture marioTexture;
Texture pinkTexture;
Texture checkerboardTexture;
Texture houseTexture;

Lights lights;

Camera camera;

Text8x8Font font;

int nFrames = 0;

//This function is called once before the first frame:
void Start()
{
    camera.position = {0, 0, 0};
    camera.rotation = {0, 0, 0};

    //lights.lightDirection = {0.41, 0.41, -0.82};
    lights.lightDirection = {0.82, 0.41, -0.41};
    lights.lightColor = {255, 255, 255, 255};
    lights.ambientColor = {255, 100, 100, 100};
    rendProps.lights = lights;

    exampleBall = LoadObjModel("./models/ball.obj");
    marioModel = LoadObjModel("./models/mario.obj");
    checkCubeModel = LoadObjModel("./models/checkCube.obj");
    houseModel = LoadObjModel("./models/house.obj");

    marioTexture = LoadTexture("./models/mario.png");
    checkerboardTexture = LoadTexture("./models/checkerboard.png");
    houseTexture = LoadTexture("./models/house.png");

    font = LoadNumbersFont("./models/font.png");
}



//This function is called once every frame:
void Update()
{
    //Clear frame:
    for(int i = 0; i < rendProps.width * rendProps.height; i++)
    {
        rendProps.pixels[i] = 0xFF0064C8; //Blue sky background
        //rendProps.pixels[i] = 0xFF000000; //Black background
        rendProps.zBuffer[i] = 1.0;
        zBufferViewPixels[i] = 0x00000000;
    }

    nFrames++;

    //Basic tests:
    //DrawPixel(rendProps, 100, 50, 0xFFFF0000);
    //DrawLine(rendProps, 300, 150, 600, 300, 0xFF00FF00);
    //DrawTriangleBarycentric(rendProps, {150, 420}, {350, 50}, {500, 470}, 0xFF0000FF, 0xFFFF00FF, 0xFF00FFFF);

    //Draw a cube with perspective:
    //DrawCubeOld(rendProps, cubePos, angle * 2, cubeScale);
    //DrawCube(rendProps, cubePos, {0, angle, 0}, {1, 1, 1});

    //Draw 3d models:
    //DrawModel(rendProps, ballPos, {0, angle, 0}, exampleBall);
    //DrawModel(rendProps, ballPos, {0, angle, 0}, marioModel);
    //DrawModelColor(rendProps, ballPos, {0, angle, 0}, {0.05, 0.05, 0.05}, marioModel, {255, 255, 0, 0});
    //DrawModelTexture(rendProps, ballPos, {0, angle, 0}, {0.05, 0.05, 0.05}, marioModel, marioTexture);

    //Change light:
    //lights.lightDirection = RotateY(lights.lightDirection, deltaTime * 4);
    //rendProps.lights = lights;

    //Checkerboard (affine vs perspective):
    //DrawModelTexture(rendProps, cubePos, {0, angle, 0}, {1, 1, 1}, checkCubeModel, checkerboardTexture);

    //House:
    DrawModelTexture(rendProps, {0, -2, 0}, {0, 0, 0}, {1.4, 1.4, 1.4}, houseModel, houseTexture);
    DrawModelTexture(rendProps, marioPos, {0, angle, 0}, {0.05, 0.05, 0.05}, marioModel, marioTexture);

    angle += 0.5 * deltaTime;
}


int main()
{
    //Canvas setup:
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\nSDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    int gameScale = 1;
    int gameWidth = 640;
    int gameHeight = 480;

    rendProps.width = gameWidth / gameScale;
    rendProps.height = gameHeight / gameScale;
    rendProps.pixels = new uint32_t[rendProps.width * rendProps.height];
    rendProps.zBuffer = new float[rendProps.width * rendProps.height];
    zBufferViewPixels = new uint32_t[rendProps.width * rendProps.height];

    SDL_Window* window = SDL_CreateWindow("Guinxu Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth, gameHeight, 0);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, rendProps.width, rendProps.height);

    //Start function, before the first frame:
    Start();

    //Main loop:
    bool running = true;
    while(running)
    {
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        float camSpd = 5.0;
        float camRotSpd = 1.0;
        float spd = 2.0;

        //Cube controls:
        /*if (keyboardState[SDL_SCANCODE_Q]) { cubePos.z += spd * deltaTime;  }
        if (keyboardState[SDL_SCANCODE_E]) { cubePos.z -= spd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_A]) { cubePos.x -= spd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_D]) { cubePos.x += spd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_S]) { cubePos.y -= spd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_W]) { cubePos.y += spd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_R]) { cubeScale = {cubeScale.x + spd * deltaTime, cubeScale.y + spd * deltaTime, cubeScale.z + spd * deltaTime}; }
        if (keyboardState[SDL_SCANCODE_F]) { cubeScale = {cubeScale.x - spd * deltaTime, cubeScale.y - spd * deltaTime, cubeScale.z - spd * deltaTime}; }
        if (keyboardState[SDL_SCANCODE_J]) { angle += 0.5 * deltaTime;  }
        if (keyboardState[SDL_SCANCODE_L]) { angle -= 0.5 * deltaTime; }*/

        //Camera controls:
        if (keyboardState[SDL_SCANCODE_W]) { camera.position.x -= camSpd * sin(camera.rotation.y) * deltaTime; camera.position.z += camSpd * cos(camera.rotation.y) * deltaTime; }
        if (keyboardState[SDL_SCANCODE_S]) { camera.position.x += camSpd * sin(camera.rotation.y) * deltaTime; camera.position.z -= camSpd * cos(camera.rotation.y) * deltaTime; }
        if (keyboardState[SDL_SCANCODE_A]) { camera.rotation.y += camRotSpd * deltaTime; }
        if (keyboardState[SDL_SCANCODE_D]) { camera.rotation.y -= camRotSpd * deltaTime; }
        rendProps.camera = camera;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { running = false; }
            if (event.type == SDL_KEYDOWN)
            {
                //Z-buffer mode toggle:
                if (event.key.keysym.sym == SDLK_z) { zBufferView = !zBufferView; }
            }
        }

        //Calculate deltaTime:
        uint32_t currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - lastFrameTime) / 1000.0;
        lastFrameTime = currentFrameTime;

        //Update function, every frame:
        Update();

        //FPS counter:
        DrawNumber8x8(rendProps, font, (int) (1.0 / deltaTime), 10, rendProps.height - 40, 0xFFFFFFFF, 4);

        //Render mode: (zBuffer / full color)
        if (zBufferView)
        {
            for(int i = 0; i < rendProps.width; i++)
            {
                for(int j = 0; j < rendProps.height; j++)
                {
                    zBufferViewPixels[i + j * rendProps.width] = expandUint8ToUint32(floatToUint8(rendProps.zBuffer[i + (rendProps.height - j) * rendProps.width]));
                }
            }
            SDL_UpdateTexture(texture, NULL, zBufferViewPixels, rendProps.width * sizeof(uint32_t));
        }
        else
        {
            SDL_UpdateTexture(texture, NULL, rendProps.pixels, rendProps.width * sizeof(uint32_t));
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    //Clear everything and exit:
    delete[] rendProps.pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
