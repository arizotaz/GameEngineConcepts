#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/structs.h>
#include <engine/tools.h>


namespace GEC {

MenuManager::MenuManager() { 
    menuList = new GEC::Menu*[0];
}
void MenuManager::Update()
{
    if (this->nextIndex != this->lastInd) {
        Menu* lastMenu = GetMenu(this->lastInd);
        Menu* newMenu = GetMenu(this->nextIndex);

        if (lastMenu != nullptr)
            lastMenu->Leave();

        this->currMenu = newMenu;
        this->currInd = this->nextIndex;

        if (this->currMenu != nullptr)
            currMenu->Open();

        this->lastInd = this->currInd;
    }

    if (this->currMenu != nullptr)
        this->currMenu->Update();
}
void MenuManager::Render()
{
    if (this->currMenu != nullptr)
        this->currMenu->Render();
}
void MenuManager::Events()
{
    if (this->currMenu != nullptr)
        this->currMenu->Events();
}
void MenuManager::AddMenu(int id, Menu* m) {
    // If the list is empty
    if (this->menus < 1) {
        this->menus = id + 1;
        this->menuList = new Menu * [this->menus];

        // Fill List with nullptr
        for (int i = 0; i < this->menus; ++i)
            this->menuList[i] = nullptr;
    }

    if (id > this->menus - 1) {
        // Resize Menu Index
        int newSize = id + 1;
        Menu** newList = new Menu * [newSize];

        // Copy everything over
        for (int i = 0; i < this->menus; ++i)
            newList[i] = this->menuList[i];

        // Initialize new slots to nullptr (IMPORTANT)
        for (int i = this->menus; i < newSize; ++i)
            newList[i] = nullptr;

        // Delete old list (correctly)
        delete[] this->menuList;

        // Set new data
        this->menus = newSize;
        this->menuList = newList;
    }

    // If the menu already exists, then we need to remove the old one
    if (this->menuList[id] != nullptr) {
        delete this->menuList[id];
        this->menuList[id] = nullptr;
    }

    m->SetManager(this);

    // Add the new menu
    this->menuList[id] = m;
}
void MenuManager::RemoveMenu(int id)
{
    delete this->menuList[id];
    this->menuList[id] = nullptr;
}
bool MenuManager::Exists(int id)
{
    return GetMenu(id) != nullptr;
}


Menu* MenuManager::GetMenu(int id)
{
    if (id < 0) return nullptr;
    if (this->menus <= id)
        return nullptr;
    return this->menuList[id];
}

MenuManager::~MenuManager()
{
    for (int i = 0; i < this->menus; ++i) {
        delete this->menuList[i];
    }
    delete[] this->menuList;
}


DeltaTime::DeltaTime() {
    lastTime = glutGet(GLUT_ELAPSED_TIME);
}
void DeltaTime::Update() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME); // milliseconds
    int delta = currentTime - lastTime;
    lastTime = currentTime;
    dtime = delta / 1000.0f;
}
float DeltaTime::Get() {
    return GEC::Tools::ClampVar<float>(dtime,0,.02f);
}

}