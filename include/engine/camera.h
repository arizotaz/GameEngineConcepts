// #############################################################################
// # camera.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/03/2025
// # Last Modification:  03/10/2025
// #############################################################################
// # World Camera Structure
// #############################################################################
// # This object serves as a camera "handler".  It is not the true camera, that
// # is handled by OpenGL.  Instead this is a stucture that stores (x,y,z) and
// # scale values to be used by OpenGL at the start of every render cycle.
// #
// # This also holds the view port size
// #############################################################################

#ifndef CAMERA_H
#define CAMERA_H 1

#include <engine/structs.h>

class Camera {
public:
    static Camera& GetInstance()
    {
        static Camera cam;
        return cam;
    }
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    float X();
    float Y();

    GEC::Vector2<float, float> ViewPort();

    /**
     * Moves the position of the camera
     */
    void Move(float, float);

    /**
     * Sets the position of the camera to a specified X,Y value
     */
    void SetPos(float, float);

    /**
     * Sets the position of the camera to a specified Vector of type float
     */
    void SetPos(GEC::Vector2<float, float>);

    /**
     * Returns the current camera position as a Vctor3 of type float
     */
    GEC::Vector3<float, float, float> Position();

    /**
     * Sets the scaling of the camera
     */
    void SetScale(float);

    /**
     * Returns the scale of the camera
     */
    float GetScale() const;

    /**
     * Sets viewport size of the camera given a width and a height value
     */
    void SetScreen(float, float);

    /**
     * Sets the viewport size of the camera given a vector, where it's WXH
     */
    void SetScreen(GEC::Vector2<float, float>);

    /**
     * Sets the position and the viewport of the camera.
     * Where the first vector is position, and the second vector is screen size
     */
    void SetPosAndSize(float,float,float,float);
    /**
     * Sets the position and the viewport of the camera.
     * Where the first vector is position, and the second vector is screen size
     */
    void SetPosAndSize(GEC::Vector2<float, float>, GEC::Vector2<float, float>);
    /**
     * Sets the position and the viewport of the camera.
     * Where the first two values are (x,y) and the second two are (w,h)
     */
    void SetPosAndSize(GEC::Rect<float, float, float, float>);

    /**
     * Moves the camera towards a position
     */
    void MoveTo(float x, float y, float speed);

private:
    Camera()
        : cameraView(0, 0,0,0)
    {
    }
    ~Camera() { }
    float scale = 1;
    GEC::Rect<float,float,float,float> cameraView;
};

#endif