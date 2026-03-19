#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/input.h>
#include <engine/structs.h>
#include <iostream>
#include <stdio.h>

void GEC::Input::Keyboard::InterruptKeyDown(unsigned char key, int x, int y)
{
    if (!keys[key])
        keys_pressed[key] = 2;
    keys[key] = true;

    if (printKeys) {
        std::cout << "Key " << key << " " << "(" << (int)key << ")" << " is down";
        fflush(stdout);
    }
}

void GEC::Input::Keyboard::InterruptKeyUp(unsigned char key, int x, int y)
{
    keys[key] = false;

    if (printKeys) {
        std::cout << "Key " << key << " " << "(" << (int)key << ")" << " is up" << std::endl;
        fflush(stdout);
    }
}

void GEC::Input::Keyboard::InterruptSpecialDown(int key, int x, int y)
{
    specialKeys[key] = true;

    if (printKeys) {
        std::cout << "Key " << key << " " << "(" << (int)key << ")" << " is down" << std::endl;
        fflush(stdout);
    }
}

void GEC::Input::Keyboard::InterruptSpecialUp(int key, int x, int y)
{
    specialKeys[key] = false;

    if (printKeys) {
        std::cout << "Key " << key << " " << "(" << (int)key << ")" << " is up" << std::endl;
        fflush(stdout);
    }
}

void GEC::Input::Keyboard::PrintKeysToConsole(bool value)
{
    printKeys = value;

    std::cout << "Pressed key codes will print to console!" << std::endl;
    fflush(stdout);
}

bool GEC::Input::Keyboard::IsKeyDown(int key)
{
    return keys[key];
}

bool GEC::Input::Keyboard::IsKeyPressed(int key)
{
    return keys_pressed[key] > 0;
}

bool GEC::Input::Keyboard::IsSpecialKeyDown(int key)
{
    return specialKeys[key];
}

void GEC::Input::Keyboard::UpdateLoop()
{
    for (int i = 0; i < 256; ++i)
        if (keys_pressed[i] > 0)
            --keys_pressed[i];
}

void GEC::Input::Mouse_GLUT::PassiveMotionInterrupt(int x, int y)
{
    Mouse::GetInstance().SetPosition(x, y);
}
void GEC::Input::Mouse_GLUT::MouseFuncInterrupt(int button, int state, int x, int y)
{
    Mouse::GetInstance().SetButton(button, state == GLUT_DOWN);
}
void GEC::Input::Mouse_GLUT::MouseWheelInterrupt(int b, int dir, int x, int y)
{
    Mouse::GetInstance().SetScroll(0, dir);
}

GEC::Input::Mouse::Mouse() { }
GEC::Input::Mouse::~Mouse() { }
GEC::Vector2<float,float> GEC::Input::Mouse::Position() const { return GEC::Vector2<float,float>(x,y); }
bool GEC::Input::Mouse::LeftDown() const { return buttons[MouseButtons::LEFT]; }
bool GEC::Input::Mouse::RightDown() const { return buttons[MouseButtons::RIGHT]; }
bool GEC::Input::Mouse::LeftPressed() const { return pressedButtons[MouseButtons::LEFT] == 2; }
bool GEC::Input::Mouse::RightPressed() const { return pressedButtons[MouseButtons::RIGHT] == 2; }
float GEC::Input::Mouse::ScrollX() const { return this->scrollX; }
float GEC::Input::Mouse::ScrollY() const { return this->scrollY; }
void GEC::Input::Mouse::SetPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}
void GEC::Input::Mouse::SetButton(int b, int s)
{
    rawButtons[b] = s;
}
void GEC::Input::Mouse::SetScroll(float x, float y)
{
    this->rawScrollX = x;
    this->rawScrollY = y;
}

void GEC::Input::Mouse::Update()
{

    scrollX = 0;
    scrollY = 0;
    if (rawScrollX != 0) {
        scrollX = rawScrollX;
        rawScrollX = 0;
    }
    if (rawScrollY != 0) {
        scrollY = rawScrollY;
        rawScrollY = 0;
    }

    for (int i = 0; i < numOfButtons; ++i)
        if ((pressedButtons[i] > 1) || (pressedButtons[i] > 0 && buttons[i] == 0))
            --pressedButtons[i];

    for (int i = 0; i < numOfButtons; ++i)
        buttons[i] = rawButtons[i];

    for (int i = 0; i < numOfButtons; ++i) {
        if (buttons[i] > 0 && pressedButtons[i] == 0)
            pressedButtons[i] = 2;
    }
}

void GEC::Input::Mouse::SetCursor(std::string cursor) {
    this->cursor = cursor;
    if (cursor == "cursor")
    glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
    if (cursor == "pointer")
    glutSetCursor(GLUT_CURSOR_INFO);
    
    
}
std::string GEC::Input::Mouse::GetCursor() const {
    return cursor;
}
