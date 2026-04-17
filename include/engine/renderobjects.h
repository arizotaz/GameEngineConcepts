// #############################################################################
// # RenderObjects
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/11/2026
// # Last Modification:  02/18/2026
// #############################################################################
// # This header contains a handfull of usefull meathods for rendering items and
// # and objects to the screen
// #############################################################################
// #
// # Methods in this File include
// #
// #    ClearScreen() - Clears the screen
// #    ClearScreen(int,int,int) - Clears the screen with a specified color
// #    SetColor(float) - Sets the current color to a value of luminosity
// #                      [white-balck]
// #    SetColor(float, float) - Sets current color to luminosity with alpha
// #    SetColor(float, float, float) - Sets current color to rgb value [0-255]
// #    SetColor(float, float, float, float) - Sets current color to rgb value
// #                                           with alpha [0-255]
// #    Rect(float, float, float, float) - Draws Square at (x,y) of size (w,h)
// #                                       Origin is Center
// #    Triangle(float, float,
// #              Vector2<float,float>,
// #              Vector2<float,float>,
// #              Vector2<float,float>,
// #              float) - Draws Triangle with an origin of (x,y) and each vector
// #                       as a point offset of each vector
// #    Line(Vector2<float, float>, Vector2<float, float>, float) - Line from
// #             (x,y) to (x,y) with width of {float}
// #    Arrow(Vector2<float, float>, Vector2<float, float>, float, float)
// #           Same as line with the final float being arrow head width
// #    Image(char*, float, float, float, float) - Draws Textured Square
// #                    at (x,y) of size (w,h) Origin is Center
// #    Sprite(char*, float, float, float, float, Vector2<int,int>, int) -
// #                    Draws a sprite from a spritesheet
// #
// #
// #
// #############################################################################

#ifndef RENDEROBJECTS_H
#define RENDEROBJECTS_H 1

#include <engine/structs.h>

namespace GEC {

namespace Render {

    /**
     * Clears the Color and Depth Buffers of the OpenGL Context
     */
    void ClearScreen();
    /**
     * Clears the screen with a set color
     * @param r - Red color value [0-255]
     * @param g - Green color value [0-255]
     * @param b - Blue color value [0-255]
     */
    void ClearScreen(float r, float g, float b);

    /**
     * Sets the current color to an RGBA value
     * @param r - Red color value [0-255]
     * @param g - Green color value [0-255]
     * @param b - Blue color value [0-255]
     * @param a - Alpha value [0-255]
     */
    void SetColor(float r, float g, float b, float a);

    /**
     * Sets the current color to an RGB value
     * @param r - Red color value [0-255]
     * @param g - Green color value [0-255]
     * @param b - Blue color value [0-255]
     */
    void SetColor(float r, float g, float b);

    /**
     * Sets the current color to an brightness value with alpha
     * @param l - Brightness [0-255]
     * @param a - Alpha value [0-255]
     */
    void SetColor(float l, float a);

    /**
     * Sets the current color to an brightness
     * @param l - Brightness [0-255]
     */
    void SetColor(float l);

    void SetColor(GEC::Rect<float, float, float, float> c);
    void SetColor(GEC::Rect<float, float, float, float>* c);

    /**
     * Draws a Square with the current color
     * Object origin is CENTER
     * @param x - position on the x axis
     * @param y - position on the y axis
     * @param width - width of the object
     * @param height - height of the object
     */
    void Rect(float x, float y, float width, float height);
    void Rect(float x, float y, float z, float width, float height);

    /**
     * Draws a Triangle with the current color
     * Object origin is CENTER and points are offsets of (x,y)
     * @param x - position on the x axis
     * @param y - position on the y axis
     * @param point1 - first point
     * @param point2 - second point
     * @param point3 - third point
     * @param rotation - Rotation in Degrees
     */
    void Triangle(float x, float y, Vector2<float, float> point1, Vector2<float, float> point2, Vector2<float, float> point3, float rotation);
    void Triangle(float x, float y, float z, Vector3<float, float, float> point1, Vector3<float, float, float> point2, Vector3<float, float, float> point3, float rotation);

    /**
     * Draws a simple line in the current color
     * from position1 to position2
     * @param from - source coordinate
     * @param to - destination coordinate
     * @param thickness - Pixel Thickness of the line
     */
    void Line(Vector2<float, float> from, Vector2<float, float> to, float thickness);
    void Line(Vector3<float, float, float> from, Vector3<float, float, float> to, float thickness);

    /**
     * Draws a simple arrow in the current color
     * from position1 to position2
     * @param from - source coordinate
     * @param to - destination coordinate
     * @param line_thickness - Pixel Thickness of the line
     * @param head_size - Size of the arrow head
     */
    void Arrow(Vector2<float, float> from, Vector2<float, float> to, float line_thickness, float head_size);
    void Arrow(Vector3<float, float, float> from, Vector3<float, float, float> to, float line_thickness, float head_size);

    /**
     * Draws a textured square with the current color
     * Object origin is CENTER
     *
     * Texture is the ID as registered in GEC::TextureEngine
     *
     * @param id - Texture ID
     * @param x - position on the x axis
     * @param y - position on the y axis
     * @param width - width of the object
     * @param height - height of the object
     */
    void Image(const char* texID, float x, float y, float width, float height);
    void Image(const char* texID, float x, float y, float z, float width, float height);

    /**
     * Draws a specific cell of a sprite sheet with the current color
     * Object origin is CENTER
     *
     * Texture is the ID as registered in GEC::TextureEngine
     *
     *
     * @param id - Texture ID
     * @param x - position on the x axis
     * @param y - position on the y axis
     * @param z - position on the z axis
     * @param width - width of the object
     * @param height - height of the
     * @param cell_position - the cell index to draw (ie cell (2,3) )
     * @param num_of_sprites_per_row - The number of sprites in each row of the spritesheet
     */
    void Sprite(const char* texID, float x, float y, float width, float height, GEC::Vector2<int, int> cell_position, int num_of_sprites_per_row);
    void Sprite(const char* texID, float x, float y, float z, float width, float height, GEC::Vector2<int, int> cell_position, int num_of_sprites_per_row);
}

}

#endif