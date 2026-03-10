// #############################################################################
// # input.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/14/2025
// # Last Modification:  02/17/2025
// #############################################################################
// # Handles all types of supported inputs
// #############################################################################
// # 
// # This file supports keyboard and mouse inputs, as well as declares the
// # callback functions that Glut uses when handling these system calls
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// # 
// #
// #############################################################################


#ifndef KeyboardInput_h
#define KeyboardInput_h 1

#include <stdbool.h>

namespace GEC {
namespace Input {
    namespace Keyboard {

        // static list of all keys raw state
        static bool keys[256];

        // static list of keys pressed state (0 = not pressed)
        //                                   (1 = Waiting for key release)
        //                                   (2 = KeyPressed)
        static int keys_pressed[256];
        static bool specialKeys[256];
        static bool printKeys = false;

        /**
         * Main Update loop to process single key press events
         */
        void UpdateLoop();

        /**
         * Glut Interrupt when a key is pressed
         * @param {key} - The char value of the pressed key
         * @param {x} - Part of glut interrupt, but not used
         * @param {y} - Part of glut interrupt, but not used
         */
        void InterruptKeyDown(unsigned char key, int x, int y);

        /**
         * Glut Interrupt when a key is release
         * @param {key} - The char value of the released key
         * @param {x} - Part of glut interrupt, but not used
         * @param {y} - Part of glut interrupt, but not used
         */
        void InterruptKeyUp(unsigned char key, int x, int y);

        /**
         * Glut Interrupt when a special key is pressed
         * Special Keys are refered to as (esc, alt, ctrl, arrows, .etc)
         * @param {key} - The keycode of the pressed key
         * @param {x} - Part of glut interrupt, but not used
         * @param {y} - Part of glut interrupt, but not used
         */
        void InterruptSpecialDown(int key, int x, int y);

        /**
         * Glut Interrupt when a special key is released
         * Special Keys are refered to as (esc, alt, ctrl, arrows, .etc)
         * @param {key} - The keycode of the released key
         * @param {x} - Part of glut interrupt, but not used
         * @param {y} - Part of glut interrupt, but not used
         */
        void InterruptSpecialUp(int key, int x, int y);

        /**
         * Function for Keyboard Debugging.
         * When true, the Keyboard Manager will output pressed and released
         * keys to the console.
         * @param {value} - Set debug mode ON or OFF
         */
        void PrintKeysToConsole(bool value);

        /**
         * Returns true, when a given Key is depressed
         * This function is only for letter keys (a,b,c,d,q,.etc)
         * @param {key} - char letter of the key
         * @return {key_depressed} - The key is currently depressed
         */
        bool IsKeyDown(int key);

        /**
         * Returns true, for only one frame when a key is depressed
         * This function is only for letter keys (a,b,c,d,q,.etc)
         * @param {key} - char letter of the key
         * @return {key_depressed} - The key has been pressed
         */
        bool IsKeyPressed(int key);

        /**
         * Returns true, when a given Key is depressed
         * This function is for special keys (esc, alt, ctrl, .etc)
         * @param {key} - char letter of the key
         * @return {key_depressed} - The key is currently depressed
         */
        bool IsSpecialKeyDown(int key);
    }

    /**
     * Enum of Mouse Keys
     * 0 = LEFT
     * 1 = MIDDLE
     * 2 = RIGHT
     */
    enum MouseButtons {
        LEFT,
        MIDDLE,
        RIGHT
    };

    /**
     * Structure to handle and hold mouse pointer, button, and scroll events
     */
    class Mouse {
    public:

        /**
         * Singleton interface
         */
        static Mouse& GetInstance()
        {
            static Mouse mouse;
            return mouse;
        }

        /**
         * Main Update loop for processing movement and one-shot presses
         */
        void Update();

        /**
         * Internal Usage - For use with callbacks
         * Forward the (x,y) position of the mouse to this structure
         * @param {x} - X position of the pointer
         * @param {y} - Y position of the pointer
         */
        void SetPosition(int, int);

        /**
         * Internal Usage - For use with callbacks
         * Forward the button states of the mouse to this structure
         * @param {button} - button 0-2 of the mouse (see Input::MouseButtons)
         * @param {state} - 0 = non pressed, 1 = pressed
         */
        void SetButton(int, int);

        /**
         * Internal Usage - For use with callbacks
         * Forward the mouse wheel states to this structure
         * @param {scrollX} - the X Scroll Factor
         * @param {scrollY} - the Y Scroll Factor
         */
        void SetScroll(float, float);

        /**
         * @return {bool} - If left mouse button is depressed
         */
        bool LeftDown() const;

        /**
         * @return {bool} - If right mouse button is depressed
         */
        bool RightDown() const;

        /**
         * Returns true for a single frame once the Left Mouse Button is pressed
         * @return {bool} - If left mouse button is pressed
         */
        bool LeftPressed() const;

        /**
         * Returns true for a single frame once the Right Mouse Button is pressed
         * @return {bool} - If right mouse button is pressed
         */
        bool RightPressed() const;

        /**
         * @return {float} - current scroll input on the x axis
         */
        float ScrollX() const;
        /**
         * @return {float} - current scroll input on the y axis
         */
        float ScrollY() const;
    private:
        Mouse();
        ~Mouse();
        float x, y;
        const int numOfButtons = 3;
        bool rawButtons[3];
        bool buttons[3];
        int pressedButtons[3];
        float scrollX, scrollY, rawScrollX,rawScrollY;
    };

    namespace Mouse_GLUT {
        /**
         * GLUT Interrupt to capture mouse movement
         * @param {x} - x position of the mouse
         * @param {y} - y position of the mouse
         */
        void PassiveMotionInterrupt(int x, int y);

        /**
         * GLUT Interrupt to capture mouse button state
         * @param {button} - 0-2, see Input::MouseButtons
         * @param {state} - GLUT_DOWN or GLUT_UP
         * @param {x} - x position of the mouse
         * @param {y} - y position of the mouse
         */
        void MouseFuncInterrupt(int button, int state, int x, int y);

        /**
         * GLUT Interrupt to capture mouse wheel state
         * @param {button} - In Glut Callback but unused
         * @param {dir} - the current scroll factor [-1,1]
         * @param {x} - In Glut Callback but unused
         * @param {y} - In Glut Callback but unused
         */
        void MouseWheelInterrupt(int b, int dir, int x, int y);
    }
}
}

#endif