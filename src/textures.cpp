#include "textures.h"

using namespace std;

//Returns a "Texture" struct with all the data extracted from the "path" file
Texture LoadTexture(string path)
{
	SDL_Surface* loaded = IMG_Load(path.c_str());
	if (!loaded) { printf("Error loading image: %s\n", IMG_GetError()); exit(1); }
	SDL_Surface* surface = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!surface) { printf("Error loading surface: %s\n", SDL_GetError()); exit(1); }

    Texture result;

    int width = surface->w;
    int height = surface->h;
    uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);

    result.width = width;
    result.height = height;
    result.pixels = pixels;

    return result;
}

//Returns a color in format 0xAARRGGBB from the (x,y) pixel coordinates of a Texture struct;
uint32_t GetTexturePixelColor(Texture texture, int x, int y)
{
	return texture.pixels[x + y * texture.width];
}

//Returns a color in format 0xAARRGGBB from the normalized (x,y) coordinates of a Texture struct:
uint32_t GetTexturePixelColor01(Texture texture, float x, float y)
{
	float u = x * (texture.width - 1);
	float v = (1.0 - y) * (texture.height - 1);
	return texture.pixels[(int)u + (int)v * texture.width];
}

//Returns a Text8x8Font struct with all the font data of the numbers drawn in the "path" PNG file:
Text8x8Font LoadNumbersFont(string path)
{
	Text8x8Font font;

	SDL_Surface* loaded = IMG_Load(path.c_str());
	if (!loaded) { printf("Error loading image: %s\n", IMG_GetError()); return font; }
	SDL_Surface* surface = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!surface) { printf("Error loading surface: %s\n", SDL_GetError()); return font; }

    uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);

    int width = 40;
    int height = 16;
    int size = 8; //each number has 8x8 pixels
    int cols = 5;

    Texture numbersTexture;
    numbersTexture.width = width;
    numbersTexture.height = height;
    numbersTexture.pixels = pixels;

    for (int i = 0; i < 10; i++)
    {
    	Text8x8Number number;

    	int row = 0;
    	if (i >= cols) { row = 1; }
    	int col = i % cols;

		for (int x = 0; x < size; x++)
		{
		   	for (int y = 0; y < size; y++)
		   	{
		   		number.number[x + y * size] = GetTexturePixelColor(numbersTexture, x + col * size, y + row * size);
		   	}
		}
		font.numbers[i] = number;
	}
    return font;
}

//Draw the pixels of a digit using the specified font:
void DrawDigit8x8(Renderer& renderer, Text8x8Font font, int digit, int x, int y, uint32_t color, int scale)
{
	Text8x8Number& digitToDraw = font.numbers[digit];

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if ((digitToDraw.number[i + j * 8] & 0x00FFFFFF) != 0x00000000)
			{
				for(int scalex = 0; scalex < scale; scalex++)
				{
					for(int scaley = 0; scaley < scale; scaley++)
					{
						DrawPixel(renderer, x + i * scale + scalex , y + (8 - j) * scale + scaley, color);
					}
				}
			}
		}
	}
}

//Draw a full number composed by multiple digit using the specified font:
void DrawNumber8x8(Renderer& renderer, Text8x8Font font, int number, int x, int y, uint32_t color, int scale)
{
	string text = to_string(number);
	for(int i = 0; i < text.size(); i++)
	{
		int digit = text[i] - '0';

		DrawDigit8x8(renderer, font, digit, x + i * 8 * scale, y, color, scale);
	}
}