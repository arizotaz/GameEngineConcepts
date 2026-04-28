#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/game_structs.h>
#include <ranges>

namespace GEC {
namespace Game {

    Scene::Scene()
    {
    }
    void Scene::Start()
    {
        std::cout << "size: " << objects.size() << std::endl;
        for (auto child : objects) {
            child->RunChildStart();
        }
    }
    void Scene::Update()
    {
        for (auto child : objects) {
            child->RunChildUpdate();
        }
    }
    void Scene::Render()
    {
        for (auto child : objects) {
            child->RunChildDraw();
        }
    }
    void Scene::Load() { }
    std::vector<GameObject*> Scene::Objects() const
    {
        return objects;
    }
    void Scene::AddObject(GameObject* obj) { this->objects.push_back(obj); }
    void Scene::AddObject(GameObject* obj, float x, float y) { this->objects.push_back(obj);obj->position->Set(x,y); }
    GameObject* Scene::RemoveObject(GameObject* obj)
    {
        int index = 0;
        for (int i = 0; i < objects.size(); ++i) {
            if (objects[i] == obj) {
                index = i;
                break;
            }
        }
        return RemoveObject(index);
    }
    GameObject* Scene::RemoveObject(int i)
    {
        GameObject* obj = objects[i];
        auto it = objects.begin() + i;
        this->objects.erase(it);
        return obj;
    }
    Scene::~Scene()
    {
        for (auto child : objects) {
            delete child;
        }
        objects.clear();
    }

    /**
     * GameObject Constructors are declared in the header file
     */

    void GameObject::Draw()
    {
        glPushMatrix();
        glTranslatef(position->First(), position->Second(), 0);
        glRotatef(rotation->First(), rotation->Second(), 0, 1);
        glScalef(scale->First(), scale->Second(), 1);
        Render();
        glPopMatrix();
    }
    std::string GameObject::Type() const
    {
        return typeIdentifier;
    }
    GEC::Vector2<float, float>* GameObject::Position() { return position; }
    GEC::Vector2<float, float>* GameObject::Rotation() { return rotation; }
    GEC::Vector2<float, float>* GameObject::Scale() { return scale; }
    GameObject* GameObject::Parent() { return parent; }
    std::vector<GameObject*> GameObject::Children() const
    {
        return children;
    }
    void GameObject::AddChild(GameObject* obj)
    {
        this->children.push_back(obj);
    }
    GameObject* GameObject::RemoveChild(GameObject* obj)
    {
        int index = 0;
        for (int i = 0; i < children.size(); ++i) {
            if (children[i] == obj) {
                index = i;
                break;
            }
        }
        return RemoveChild(index);
    }
    GameObject* GameObject::RemoveChild(int i)
    {
        GameObject* obj = children[i];
        auto it = children.begin() + i;
        this->children.erase(it);
        return obj;
    }
    GameObject::~GameObject()
    {
        for (auto child : children) {
            delete child;
        }
        children.clear();
        delete position;
        delete rotation;
        delete scale;
    }

    void GameObject::RunChildStart()
    {
        Start();
        for (auto child : children) {
            child->RunChildStart();
        }
    }
    void GameObject::RunChildUpdate()
    {
        Update();
        for (auto child : children) {
            child->RunChildUpdate();
        }
    }
    void GameObject::RunChildDraw()
    {
        Draw();
        for (auto child : children) {
            child->RunChildDraw();
        }
    }

}
}