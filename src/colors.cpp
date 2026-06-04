#include "colors.h"

using namespace std;

//Converts a float number (range 0.0 - 1.0) into an 8-bit integer (range 0 - 255):
uint8_t floatToUint8(float value)
{
    value = clamp(value, 0.0f, 1.0f);
    return static_cast<uint8_t>(value * 255.0);
}

//Returns a grayscale color in 0xAARRGGBB format:
uint32_t expandUint8ToUint32(uint8_t value)
{
    return (static_cast<uint32_t>(value) << 24 | static_cast<uint32_t>(value) << 16 | static_cast<uint32_t>(value) << 8 | static_cast<uint32_t>(value));
}

//Converts a 0xAARRGGBB color integer into an {AA, RR, GG, BB} Color (0 - 255) struct:
Color UnpackColor(uint32_t color)
{
    Color c;
    c.a = (color >> 24) & 0xFF;
    c.r = (color >> 16) & 0xFF;
    c.g = (color >> 8) & 0xFF;
    c.b = color & 0xFF;

    return c;
}

//Converts a 0xAARRGGBB color integer into an {AA, RR, GG, BB} ColorFloat (0.0 - 1.0) struct:
ColorFloat UnpackColorFloat(uint32_t color)
{
    Color c;
    c.a = (color >> 24) & 0xFF;
    c.r = (color >> 16) & 0xFF;
    c.g = (color >> 8) & 0xFF;
    c.b = color & 0xFF;

    ColorFloat cf = ColorToColorFloat(c);

    return cf;
}

//Converts an {AA, RR, GG, BB} Color (0 - 255) struct into an 0xAARRGGBB color integer:
uint32_t PackColor(const Color& c)
{
    return (c.a << 24) | (c.r << 16) | (c.g << 8) | c.b;
}

//Converts an {AA, RR, GG, BB} ColorFloat (0.0 - 1.0) struct into an 0xAARRGGBB color integer:
uint32_t PackColorFloat(const ColorFloat& cf)
{
    Color c = ColorFloatToColor(cf);
    return (c.a << 24) | (c.r << 16) | (c.g << 8) | c.b;
}

//Lambert formula to simulate diffuse lights:
ColorFloat LambertFormula(ColorFloat lightColor, Vec3 normal, Vec3 lightDir)
{
    float dotProduct = normal.x * lightDir.x + normal.y * lightDir.y + normal.z * lightDir.z;
    if (dotProduct < 0) { dotProduct = 0; }

    ColorFloat result;
    result.a = lightColor.a * dotProduct;
    result.r = lightColor.r * dotProduct;
    result.g = lightColor.g * dotProduct;
    result.b = lightColor.b * dotProduct;

    return result;
}

//Phong formula to simulate specular reflections:
ColorFloat PhongFormula(Vec3 normal, Vec3 lightDir, Vec3 cameraDir)
{
    float shinyness = 32.0;
    float intensity = 0.8;

    float NdotL = DotProduct(normal, lightDir);
    if (NdotL <= 0) { return {1.0, 0.0, 0.0, 0.0}; }

    Vec3 r = Normalize(2.0 * NdotL * normal - lightDir);

    float RdotV = DotProduct(r, cameraDir);
    if (RdotV < 0) { RdotV = 0; }

    float phong = intensity * pow(RdotV, shinyness);
    ColorFloat result = {1.0, phong, phong, phong};
    return result;
}

//Full shader formula:
Color ApplyShaders(Renderer& renderer, Color albedo, Vec3 normal, Vec3 cameraDir)
{
    ColorFloat dirLight = LambertFormula(ColorToColorFloat(renderer.lights.lightColor), normal, renderer.lights.lightDirection);
    ColorFloat ambient = ColorToColorFloat(renderer.lights.ambientColor);
    ColorFloat phong = PhongFormula(normal, renderer.lights.lightDirection, Normalize(cameraDir));

    ColorFloat result = ClampColor(ColorToColorFloat(albedo) * (dirLight + ambient) + phong);

    return ColorFloatToColor(result);
}

//Keep ColorFloat channels inside 0.0 - 1.0 range:
ColorFloat ClampColor(ColorFloat color)
{
    ColorFloat result = color;
    if (result.a < 0) { result.a = 0; }
    if (result.r < 0) { result.r = 0; }
    if (result.g < 0) { result.g = 0; }
    if (result.b < 0) { result.b = 0; }
    if (result.a > 1) { result.a = 1; }
    if (result.r > 1) { result.r = 1; }
    if (result.g > 1) { result.g = 1; }
    if (result.b > 1) { result.b = 1; }

    return result;
}

//Add 2 colors by adding their channels respectively:
ColorFloat operator+(const ColorFloat& c1, const ColorFloat& c2)
{
    ColorFloat result;

    result.a = c1.a + c2.a;
    result.r = c1.r + c2.r;
    result.g = c1.g + c2.g;
    result.b = c1.b + c2.b;

    return result;
}

//Substract 2 colors by substracting their channels respectively:
ColorFloat operator-(const ColorFloat& c1, const ColorFloat& c2)
{
    ColorFloat result;

    result.a = c1.a - c2.a;
    result.r = c1.r - c2.r;
    result.g = c1.g - c2.g;
    result.b = c1.b - c2.b;

    return result;
}

//Multiply 2 colors by multiplying their channels respectively:
ColorFloat operator*(const ColorFloat& c1, const ColorFloat& c2)
{
    ColorFloat result;

    result.a = c1.a * c2.a;
    result.r = c1.r * c2.r;
    result.g = c1.g * c2.g;
    result.b = c1.b * c2.b;

    return result;
}

//Convert a Color (0 - 255) into a ColorFloat (0.0 - 1.0):
ColorFloat ColorToColorFloat(const Color& color)
{
    ColorFloat result;

    result.a = 1.0 * color.a / 255.0;
    result.r = 1.0 * color.r / 255.0;
    result.g = 1.0 * color.g / 255.0;
    result.b = 1.0 * color.b / 255.0;

    return result;
}

//Convert a ColorFloat (0.0 - 1.0) into a Color (0 - 255):
Color ColorFloatToColor(const ColorFloat& color)
{
    Color result;
    result.a = (uint8_t) (color.a * 255);
    result.r = (uint8_t) (color.r * 255);
    result.g = (uint8_t) (color.g * 255);
    result.b = (uint8_t) (color.b * 255);
    return result;
}