#include <engine/game_structs.h>

namespace GEC {
namespace Game {

    Scene::Scene() {}
    void Scene::Start() {}
    void Scene::Update() {}
    void Scene::Render() {}
    Scene::~Scene() {}


    PropertyList::PropertyList() {}
    void SetInt(std::string key, float value) {}
    void SetBool(std::string key, float value) {}
    void SetFloat(std::string key, float value) {}
    void SetString(std::string key, float value) {}
    int GetInt(std::string key) {}
    bool GetBool(std::string key) {}
    float GetFloat(std::string key) {}
    std::string GetString(std::string key) {}
    std::variant<bool,int,float,std::string> GetValue() {}
    std::string GetType(std::string key) {}
    std::vector<std::string> GetKeys() {}
    bool RemoveEntry(std::string key) {}


    /**
     * Constructors are declared in the header file
     */
    

}
}