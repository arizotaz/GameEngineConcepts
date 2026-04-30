#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/game_structs.h>
#include <engine/serialization.h>
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
    void Scene::AddObject(GameObject* obj, float x, float y)
    {
        this->objects.push_back(obj);
        obj->position->Set(x, y);
    }
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
    void Scene::Serialize(std::ostream& out) const
    {
        GEC::Serial::WriteString(out, "com.arizotaz.gec.engine.scene.start");
        GEC::Serial::Write(out, serializeVersion);
        size_t childCount = objects.size();
        GEC::Serial::Write(out, childCount);
        for (auto child : objects) {
            child->Serialize(out);
        }
        GEC::Serial::WriteString(out, "com.arizotaz.gec.engine.scene.end");
    }
    void Scene::Deserialize(std::istream& in)
    {
        std::string header;
        GEC::Serial::ReadString(in, header);
        if (header != "com.arizotaz.gec.engine.scene.start")
            throw std::runtime_error("This is not a valid scene file");

        int sceneVersion;
        GEC::Serial::Read(in, sceneVersion);
        if (sceneVersion != this->serializeVersion)
            throw std::runtime_error("Scene version is not supported");

        size_t count;
        GEC::Serial::Read(in, count);
        objects.clear();

        for (size_t i = 0; i < count; i++) {
            objects.push_back(DeserializeGameObject(in));
        }

        std::string footer;
        GEC::Serial::ReadString(in, footer);
        if (footer != "com.arizotaz.gec.engine.scene.end")
            throw std::runtime_error("This is not a completed scene file");
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

    void GameObject::Serialize(std::ostream& out) const
    {

        GEC::Serial::WriteString(out, typeIdentifier);
        GEC::Serial::WriteString(out, name);

        GEC::Serial::Write(out, position->First());
        GEC::Serial::Write(out, position->Second());

        GEC::Serial::Write(out, rotation->First());
        GEC::Serial::Write(out, rotation->Second());

        GEC::Serial::Write(out, scale->First());
        GEC::Serial::Write(out, scale->Second());

        WriteObject(out);

        size_t childCount = children.size();
        GEC::Serial::Write(out, childCount);
        for (auto child : children) {
            child->Serialize(out);
        }
    }

    GameObject* DeserializeGameObject(std::istream& in)
    {
        std::string type;
        GEC::Serial::ReadString(in, type);

        GameObject* obj = GameObjectFactory::Create(type);

        GEC::Serial::ReadString(in, obj->name);

        float x, y;

        GEC::Serial::Read(in, x);
        GEC::Serial::Read(in, y);
        obj->Position()->Set(x, y);

        GEC::Serial::Read(in, x);
        GEC::Serial::Read(in, y);
        obj->Rotation()->Set(x, y);

        GEC::Serial::Read(in, x);
        GEC::Serial::Read(in, y);
        obj->Scale()->Set(x, y);

        obj->ReadObject(in);

        size_t childCount;
        GEC::Serial::Read(in, childCount);

        for (size_t i = 0; i < childCount; i++) {
            GameObject* child = DeserializeGameObject(in);
            obj->AddChild(child);
        }

        return obj;
    }

}
}