// #############################################################################
// # tools.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/12/2026
// # Last Modification:  03/10/2026
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################

#ifndef tools_h
#define tools_h 1
#include <engine/structs.h>
#include <iostream>

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
    float Distance(Vector2<float, float> a, Vector2<float, float> b);
    /**
     * Simple Distance Formula
     */
    float Distance(float x1, float y1, float x2, float y2);

    /**
     * Clamps a value to a specific range
     * @param var - the value to clamp
     * @param min - the minimum value to clamp to
     * @param max - the maximum value to clamp to
     * @returns the value with clamps applied
     */
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

    // https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    bool StrHasEnding(std::string const& fullString, std::string const& ending);
}
}

#endif