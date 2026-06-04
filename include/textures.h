using namespace std;

#include <stdint.h>
#include <string>
#include <SDL2/SDL_image.h>

#include "types.h"
#include "draw2d.h"

Texture LoadTexture(string path);
uint32_t GetTexturePixelColor(Texture texture, int x, int y);
uint32_t GetTexturePixelColor01(Texture texture, float x, float y);

Text8x8Font LoadNumbersFont(string path);
void DrawDigit8x8(Renderer& renderer, Text8x8Font font, int digit, int x, int y, uint32_t color, int scale);
void DrawNumber8x8(Renderer& renderer, Text8x8Font font, int number, int x, int y, uint32_t color, int scale);