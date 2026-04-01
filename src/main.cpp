// #############################################################################
// # main.cpp
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/03/2025
// # Last Modification:  02/18/2025
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################
// #
// # This is a simple demonstration of GLUT functioning using STB_Image.H for
// # loading images and MiniAudio.h for playing audio. The repositories for
// # these libraries can be found below.
// #
// #############################################################################
// #
// # For GLUT/OpenGL, the operating system should have GLUT in some form already
// # installed.  However, freeglut exists as the modern implementation of GLUT.
// #
// # GLUT
// #     MacOS:
// https://developer.apple.com/library/archive/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_drawing/opengl_drawing.html
// #     Windows & Linux:
// https://www.opengl.org/resources/libraries/glut/glut_downloads.php
// #
// # FreeGLUT:
// #	https://freeglut.sourceforge.net/docs/api.php
// #
// #############################################################################
// #
// # This file will setup much of GLUT.  Getting some output to the screen
// # The following is executed:
// #     Create a windows with GLUT's API
// #     Create a display update timer
// #     Create Keyboard Interrupts
// #     Create Mouse Interrupts
// #     Declare the Main Loop funcitons
// #     Initialize Main Application Object
// #
// #############################################################################

// Libraries

// Graphics
#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

// MacOS Specific Definitions for VSync
#if defined(__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#include <OpenGL/OpenGL.h>
#endif

// STD
#include <math.h>
#include <stdio.h>

#include <iostream>

// Custom includes
#include <engine/input.h>
#include <engine/structs.h>
#include <game/deltatime.h>
#include <game/game.h>
#include <game/global_states.h>
#include <vector>

// Expose the VSync Extension on Windows
#if defined(_WIN32) && defined(use_freeglut)
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
#endif

// Frame list to store FPS Averages
std::vector<float> fpsList;

/** Returns the current FPS based on the last frame time */
float FPS() {
    return 1.0f/(GetMainDeltaTime()/1000.0f);
}

/** Returns the average FPS of the client over the last 240 Frames */
float FPS_AVERAGE() {
    fpsList.push_back(FPS());
    while (fpsList.size() > 240) 
        fpsList.erase(fpsList.begin());

    float total = 0;
    for (int i = 0; i < fpsList.size(); ++i) {
        total += fpsList[i];
    }

    total /= (float)fpsList.size();
    return total;
}


/** Returns a pointer to the global font */
GEC::TextRender::Font* GetGlobalFont() {
    if (!globalFont)
    globalFont = new GEC::TextRender::Font(RESOURCES_PATH "arial.ttf", 48);
    return globalFont;
}

/** Implement GameRunning Accessor */
bool GameRunning()
{
    return gameRunning;
}

/** Implement GameRunning CallBack */
void CloseCallBack()
{
    gameRunning = false;
}
/**
 * Function Prototypes
 */

// Basic Glut
void MainLoop();
void GlutLoopTimer(int);

// Extended Functionality
void UpdateViewPort();
void CenterWindowOnScreen();

// Last width and height of the window, used to resize viewport
int lastWinW = 0, lastWinH = 0;

// The main app code
GEC::Process* pro = nullptr;

// App Name for Debuging
const char* appName = "Assignment";

/**
 * Main entry point of the application
 */
int main(int argc, char** argv)
{
    std::cout << "Starting";
    fflush(stdout);

    pro = new Assignment2();

    int winW = 1280;
    int winH = 720;

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // RGB mode

    // Setup then create the window
    glutInitWindowSize(winW, winH); // window size
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Transform Example");

// Initialize Glad to use OpenGL Extensions
#if defined(use_freeglut)
    if (!gladLoadGL()) {
    }

    // Enable VSync
#if defined(_WIN32)
    // Enable on Windows
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(1);
    }
#endif
#if defined(__APPLE__)
    // Enable on Mac
    GLint swapInterval = 1;
    CGLContextObj ctx = CGLGetCurrentContext();
    CGLSetParameter(ctx, kCGLCPSwapInterval, &swapInterval);

#endif
#endif

    // Set Window to be the focused process on mac
    // otherwise it starts behind the current window
#ifdef __APPLE__
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    SetFrontProcess(&psn);
#endif

    // Clear screen
    glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen

    // Sets up the viewport
    UpdateViewPort();

    // Center Window
    CenterWindowOnScreen();

    // Debug Ifo
    std::cout << "[" << appName << "]" << " Initializing App" << std::endl;

    // Run Game Start Loop
    pro->Start();

    std::cout << "[" << appName << "]" << " Creating Callbacks" << std::endl;
    glutIgnoreKeyRepeat(1);
    // Set Key Callbacks
    glutKeyboardFunc(GEC::Input::Keyboard::InterruptKeyDown);
    glutKeyboardUpFunc(GEC::Input::Keyboard::InterruptKeyUp);

    // Set Special Key Callbacks
    glutSpecialFunc(GEC::Input::Keyboard::InterruptSpecialDown);
    glutSpecialUpFunc(GEC::Input::Keyboard::InterruptSpecialUp);

    // Mouse Move Callback
    glutPassiveMotionFunc(GEC::Input::Mouse_GLUT::PassiveMotionInterrupt);

    // Mouse Click Callback
    glutMouseFunc(GEC::Input::Mouse_GLUT::MouseFuncInterrupt);

#if defined(use_freeglut)
    // Mouse Scroll Callback (Only works in freeglut)
    glutMouseWheelFunc(GEC::Input::Mouse_GLUT::MouseWheelInterrupt);
#endif

    // Deplare the Main Loop Function(s)
    glutDisplayFunc(MainLoop);
    glutIdleFunc(MainLoop);

    // Setup a timer to run the main loop at an interval
    // (This call will call the GlutLoopTimer() function immediately)
    glutTimerFunc(0, GlutLoopTimer, 0);

    // Debug info
    std::cout << "[" << appName << "]" << " Started Main Loop" << std::endl;
    fflush(stdout);

#if defined(use_freeglut)
    // FreeGlut allows for manual updating of the main loop
    // So we can call the main loop in a while loop and tell the loop when to stop

    // Tell FreeGlut to do nothing on window X press
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    // Run callback when window X is pressed
    glutCloseFunc(CloseCallBack);

    // Our loop, calls the funtion in glutDisplayFunc repeatedly until running = false
    while (GameRunning())
        glutMainLoopEvent();

#else
    // Start the Glut Loop (Calls the funtions above repeatedly)
    glutMainLoop();
#endif

    // Debug Info
    std::cout << "[" << appName << "]" << " Closing" << std::endl;

    // Run Game exit code
    pro->Exit();

    // Exited
    std::cout << "[" << appName << "]" << " Exited" << std::endl;
    fflush(stdout);
    return 0;
}

/**
 * Main Loop Timer
 * Wait 16.7ms then generate next frame (~60FPS)
 */
void GlutLoopTimer(int v)
{
    if (GameRunning()) {
        // Runs the function specified in glutDisplayFunc
        glutPostRedisplay();

        // Call the timer again in 16.7 seconds ~ 60 times per second
        glutTimerFunc(1, GlutLoopTimer,
            v); // Creates a frame delay that is counted in miliseconds
    }
}

/**
 * Function to update the viewport to the size of the window
 */
void UpdateViewPort()
{
    // Gets the window size
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    // Sets up OpenGL Matrix Mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Create projection type (Orthographic with size of window)
    glOrtho(-winW / 2, winW / 2, -winH / 2, winH / 2, -1000.0f,
        1000.0f); // Clipping plane is set to 1000 behind camera and 1000
                  // infront, this works because ortho is cool


    // Set last size to the new size
    lastWinW = winW;
    lastWinH = winH;
}

/**
 * This is the main loop functions that was declared during window creation
 */
void MainLoop()
{
    // Updates the Main Deltatime loop
    UpdateMainDeltaTime();

    // Get the window size
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);

    // If wind changed, update the view port
    if (winW != lastWinW || winH != lastWinH) {
        UpdateViewPort();
    }

    // Update Keyboard Manager
    GEC::Input::Keyboard::UpdateLoop();

    // Update Mouse Update
    GEC::Input::Mouse::GetInstance().Update();

    // Execute the Game Loop
    pro->Update();
    pro->Render();
    pro->Events();

    // Flush GL Buffer
    glFlush();

    // Swap buffers to display the new frame
    glutSwapBuffers();
}

/**
 * Centers the window to the middle of the screen
 */
void CenterWindowOnScreen()
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    int windowX = (screenWidth - width) / 2;
    int windowY = (screenHeight - height) / 2;

    glutPositionWindow(windowX,windowY);

}