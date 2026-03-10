// #############################################################################
// # tools.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/12/2025
// # Last Modification:  03/10/2025
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################

#ifndef tools_h
#define tools_h 1
#include <engine/structs.h>

namespace GEC {
namespace Tools {
    /**
     * Computes the angle between (x1,y1) and (x2,y2)
     */
    float AngleBetween(float x1, float y1, float x2, float y2);
    /**
     * Computes the angle between coordinates a and b
     */
    float AngleBetween(Vector2<float, float> a, Vector2<float, float> b);

    /**
     * Converts Radians to Degrees
     */
    float RadiansToDegress(float rad);

    /**
     * Simple Distance Formula
     */
    float Distance(float x1, float y1, float x2, float y2);

    template <typename T>
    T ClampVar(T var, T _min, T _max)
    {
        if (var > _max) {
            var = _max;
        }

        if (var < _min) {
            var = _min;
        }

        return var;
    }
}
}

#endif