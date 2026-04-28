#ifndef GEC_ENGINE_GAME_STRUCTURES
#define GEC_ENGINE_GAME_STRUCTURES 1

#include <engine/structs.h>
#include <iostream>
#include <map>
#include <variant>
#include <vector>

// Remove this later
#include <engine/renderobjects.h>

namespace GEC {
namespace Game {

    class GameObject;
    /**
     * Scene object, used to create different "worlds".  Each scene will store a list of gameobjects and data for each
     */
    class Scene {
    public:
        // Constructor
        Scene();

        /**
         * Runs the start function of all objects in the scene
         */
        void Start();

        /**
         * Runs the update function of all objects in the scene
         */
        void Update();

        /**
         * Draws all objects in the scene
         */
        void Render();

        void Load();

        /**
         * List of all objects in the top level of the scene
         */
        std::vector<GameObject*> Objects() const;

        /**
         * Add object to the scene
         */
        void AddObject(GameObject* obj);
        void AddObject(GameObject* obj, float x, float y);

        /**
         * Remove Object from the top level of the scene
         * Destroys all children
         */
        GameObject* RemoveObject(GameObject* obj);

        /**
         * Remove Object at an index from the top level of the scene
         * Destroys all children
         */
        GameObject* RemoveObject(int i);

        // Deconstructor
        ~Scene();

    private:
        std::vector<GameObject*> objects;
    };
    /**
     * Main GameObject Base Class
     * Only for internal use, as it's a pure virtual
     */
    class GameObject {
    public:
        /**
         * Gameobject Constructor
         */
        GameObject(std::string identifier)
            : typeIdentifier(identifier.c_str())
        {
            position = new GEC::Vector2<float, float>(0, 0);
            rotation = new GEC::Vector2<float, float>(0, 0);
            scale = new GEC::Vector2<float, float>(1, 1);
        }

        GameObject(const GameObject& other)
        {
            typeIdentifier = other.typeIdentifier;

            position = other.position ? new GEC::Vector2<float, float>(*other.position) : nullptr;
            rotation = other.rotation ? new GEC::Vector2<float, float>(*other.rotation) : nullptr;
            scale = other.scale ? new GEC::Vector2<float, float>(*other.scale) : nullptr;

            // Shallow copy (hierarchy usually shouldn't be duplicated blindly)
            parent = nullptr; // safer than copying parent pointer

            children = other.children; // shallow copy of pointers

            name = other.name;
        }

        virtual GameObject* Clone() const = 0;

        /**
         * Start function, used when GameObject is first visible in the world
         */
        virtual void Start() = 0;
        /**
         * Update function, constant update, called regardless of window update
         */
        virtual void Update() = 0;
        /**
         * Only called on draw calls
         */
        virtual void Render() = 0;

        /**
         * Calls the render function with the appropriate transformations
         */
        virtual void Draw() final;

        /**
         * Returns the internal type of the object
         */
        std::string Type() const;

        /**
         * Sets the name of the object
         */
        void SetName(std::string name) { this->name = name; }

        /**
         * Returns the name of the object
         */
        std::string GetName() { return this->name; }

        /**
         * Pointer to the Position Vector of the object
         */
        GEC::Vector2<float, float>* Position();
        /**
         * Pointer to the Rotation Vector of the object
         */
        GEC::Vector2<float, float>* Rotation();
        /**
         * Pointer to the Scale Vector of the object
         */
        GEC::Vector2<float, float>* Scale();

        /**
         * Returns a pointer to the parent object
         */
        GameObject* Parent();

        /**
         * Returns all children of the object
         */
        std::vector<GameObject*> Children() const;

        /**
         * Adds a child to this object
         */
        void AddChild(GameObject* obj);

        /**
         * Remove Child from the list
         */
        GameObject* RemoveChild(GameObject* obj);

        /**
         * Remove child at a specific index
         */
        GameObject* RemoveChild(int i);

        bool Persistent() const { return persistent; }

        /**
         * I dont need to explain this
         */
        virtual ~GameObject();

        virtual void UpdatePropertiesPanel(float x, float y, float width, float height) { }
        virtual void InteractPropertiesPanel(float x, float y, float width, float height) { }
        virtual void RenderPropertiesPanel(float x, float y, float width, float height) { }

        friend class Scene;

    protected:
        const char* typeIdentifier;

        GEC::Vector2<float, float>* position;
        GEC::Vector2<float, float>* rotation;
        GEC::Vector2<float, float>* scale;
        GameObject* parent;
        std::vector<GameObject*> children;
        std::string name = "Un-named Object";
        bool persistent = false;

    private:
        void RunChildStart();
        void RunChildUpdate();
        void RunChildDraw();
    };
}
}

#endif