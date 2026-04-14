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
    void Scene::Load() {}

    Scene::~Scene()
    {
        for (auto child : objects) {
            delete child;
        }
        objects.clear();
    }

    PropertyList::PropertyList() { }
    void PropertyList::SetInt(std::string key, int value)
    {
        properties[key] = value;
    }
    void PropertyList::SetBool(std::string key, float value)
    {
        properties[key] = value;
    }
    void PropertyList::SetFloat(std::string key, float value)
    {
        properties[key] = value;
    }
    void PropertyList::SetString(std::string key, float value)
    {
        properties[key] = value;
    }
    int PropertyList::GetInt(std::string key)
    {
        return std::get<int>(properties[key]);
    }
    bool PropertyList::GetBool(std::string key)
    {
        return std::get<bool>(properties[key]);
    }
    float PropertyList::GetFloat(std::string key)
    {
        return std::get<float>(properties[key]);
    }
    std::string PropertyList::GetString(std::string key)
    {
        return std::get<std::string>(properties[key]);
    }
    std::variant<bool, int, float, std::string> PropertyList::GetValue(std::string key)
    {
        return properties[key];
    }
    std::string PropertyList::GetType(std::string key)
    {
        switch (properties[key].index()) {
        case 1:
            return "bool";
        case 2:
            return "int";
        case 3:
            return "float";
        case 4:
            return "string";
        default:
            return "na";
        }
    }
    std::vector<std::string> PropertyList::GetKeys()
    {
        std::vector<std::string> keys;
        for (auto const& element : properties) {
            keys.push_back(element.first);
        }
        return keys;
    }
    bool PropertyList::RemoveEntry(std::string key)
    {
        properties.erase(key);
    }

    /**
     * GameObject Constructors are declared in the header file
     */

    void GameObject::Draw()
    {
        glBegin(GL_QUADS);

        glTranslatef(position->First(), position->Second(), position->Third());
        glRotatef(rotation->First(), rotation->Second(), rotation->Third(), 1);
        glScalef(scale->First(), scale->Second(), scale->Third());

        Render();

        glEnd();
    }
    std::string GameObject::Type() const
    {
        return typeIdentifier;
    }
    GEC::Vector3<float, float, float>* GameObject::Position() { return position; }
    GEC::Vector3<float, float, float>* GameObject::Rotation() { return rotation; }
    GEC::Vector3<float, float, float>* GameObject::Scale() { return scale; }
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
        delete pList;
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