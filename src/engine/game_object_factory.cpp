#include <engine/game_structs.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace GEC {
namespace Game {

    std::unordered_map<std::string, GameObjectFactory::Creator>& GameObjectFactory::Registry()
    {
        static std::unordered_map<std::string, Creator> instance;
        return instance;
    }

    void GameObjectFactory::Register(const std::string& type, Creator creator)
    {
        Registry()[type] = creator;
    }

    GameObject* GameObjectFactory::Create(const std::string& type)
    {
        auto it = Registry().find(type);
        if (it == Registry().end()) {
            throw std::runtime_error("Unknown GameObject type: " + type);
        }
        return it->second();
    }

}
}