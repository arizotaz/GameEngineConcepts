#ifndef GEC_ENGINE_GAME_STRUCTURES
#define GEC_ENGINE_GAME_STRUCTURES 1

#include <engine/structs.h>
#include <iostream>
#include <vector>
#include <map>
#include <variant>


namespace GEC {
namespace Game {

    /**
     * Scene object, used to create different "worlds".  Each scene will store a list of gameobjects and data for each
     */
    class Scene {
    public:
        Scene();
        void Start();
        void Update();
        void Render();
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
        void SetInt(std::string key, float value);
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
        std::variant<bool,int,float,std::string> GetValue();


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
         * Returns the internal type of the object
         */
        std::string Type() {
            return typeIdentifier;
        }

        /**
         * Pointer to the Position Vector of the object
         */
        GEC::Vector3<float,float,float>* Position() { return position; }
        /**
         * Pointer to the Rotation Vector of the object
         */
        GEC::Vector3<float,float,float>* Rotation() { return rotation; }
        /**
         * Pointer to the Scale Vector of the object
         */
        GEC::Vector3<float,float,float>* Scale()    { return scale; }

        /**
         * Returns a pointer to the parent object
         */
        GameObject* Parent() { return parent; }

        /**
         * Returns all children of the object
         */
        std::vector<GameObject*> Children() const {
            return children;
        }

        /**
         * Adds a child to this object
         */
        void AddChild(GameObject* obj);

        /**
         * I dont need to explain this
         */
        virtual ~GameObject() { 
            delete pList;
            delete position;
            delete rotation;
            delete scale;
        }

    protected:
        const char* typeIdentifier;
        PropertyList* pList = nullptr;

        GEC::Vector3<float, float, float>* position;
        GEC::Vector3<float, float, float>* rotation;
        GEC::Vector3<float, float, float>* scale;
        GameObject* parent;
        std::vector<GameObject*> children;
    };
}
}

#endif