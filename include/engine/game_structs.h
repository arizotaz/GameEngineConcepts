#ifndef GEC_ENGINE_GAME_STRUCTURES
#define GEC_ENGINE_GAME_STRUCTURES 1

#include <engine/structs.h>
#include <iostream>
#include <vector>
#include <map>
#include <variant>


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

        // Deconstructor
        ~Scene();

    private:
        std::vector<GameObject*> objects;
    };
    class PropertyList {
        public:

        /**
         * Property List Default Constructor
         */
        PropertyList();

        /**
         * Sets the value of the key to an integer
         */
        void SetInt(std::string key, int value);
        /**
         * Sets the value of the key to a boolean
         */
        void SetBool(std::string key, float value);
        /**
         * Sets the value of the key to a float
         */
        void SetFloat(std::string key, float value);
        /**
         * Sets the value of the key to a string
         */
        void SetString(std::string key, float value);
        

        /**
         * Returns the integer value of the key
         */
        int GetInt(std::string key);
        /**
         * Returns the boolean value of the key
         */
        bool GetBool(std::string key);
        /**
         * Returns the float value of the key
         */
        float GetFloat(std::string key);
        /**
         * Returns the string value of the key
         */
        std::string GetString(std::string key);
        /**
         * Returns the raw value of the object
         */
        std::variant<bool,int,float,std::string> GetValue(std::string key);


        /**
         * Returns the type as a readable string
         */
        std::string GetType(std::string key);

        /**
         * Returns a vector with all keys in the list
         */
        std::vector<std::string> GetKeys();

        /**
         * Removes a key from the list
         */
        bool RemoveEntry(std::string key);

        private:
        std::map<std::string,std::variant<bool, int, float, std::string>> properties;
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
        GameObject(std::string identifier) : typeIdentifier(identifier.c_str()) {
            pList = new PropertyList();
            position = new GEC::Vector3<float,float,float>(0,0,0);
            rotation = new GEC::Vector3<float,float,float>(0,0,0);
            scale = new GEC::Vector3<float,float,float>(0,0,0);
        }

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
         * Pointer to the Position Vector of the object
         */
        GEC::Vector3<float,float,float>* Position();
        /**
         * Pointer to the Rotation Vector of the object
         */
        GEC::Vector3<float,float,float>* Rotation();
        /**
         * Pointer to the Scale Vector of the object
         */
        GEC::Vector3<float,float,float>* Scale();

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

        /**
         * I dont need to explain this
         */
        virtual ~GameObject();

        friend class Scene;

    protected:

        const char* typeIdentifier;
        PropertyList* pList = nullptr;

        GEC::Vector3<float, float, float>* position;
        GEC::Vector3<float, float, float>* rotation;
        GEC::Vector3<float, float, float>* scale;
        GameObject* parent;
        std::vector<GameObject*> children;

        private:
        void RunChildStart();
        void RunChildUpdate();
        void RunChildDraw();
    };
}
}

#endif