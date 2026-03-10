#include <engine/structs.h>
#include <engine/tools.h>
#include <math.h>

float GEC::Tools::AngleBetween(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return atan2(dy, dx); // radians
}

float GEC::Tools::AngleBetween(Vector2<float, float> a, Vector2<float, float> b)
{
    return AngleBetween(a.First(), a.Second(), b.First(), b.Second());
}

float GEC::Tools::RadiansToDegress(float rad)
{
    return rad * 180.0f / 3.14159265f;
}

float GEC::Tools::Distance(float x1, float y1, float x2, float y2)
{
    return (float)hypot((double)(x1 - x2), (double)(y1 - y2));
}