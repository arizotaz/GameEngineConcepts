// #############################################################################
// # game.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/03/2025
// # Last Modification:  03/05/2025
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################
// #
// # Handle the spawning, updating, and death of different entities
// #
// #############################################################################
#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP 1

#include <iostream>
#include <vector>

#include <engine/structs.h>

#include <game/level.h>

class LevelContainer;
class EntityManager;

class Entity {
public:
    Entity(float, float, float, float);
    virtual void OnSpawn() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void OnDeath() = 0;

    ~Entity();
    GEC::Vector2<float, float> Position() const;
    GEC::Vector2<float, float> Size() const;

    void Kill();
    bool IsDead() const;
    float Health() const;
    void SetHealth(float);

    unsigned int GetID() const;

    friend EntityManager;

protected:
    unsigned int id;
    float health = 1;
    bool isDead = false;
    bool spawned = false;
    GEC::Rect<float, float, float, float>* position_size;

private:
    void SetID(unsigned int);
};

class EntityManager {
public:
    EntityManager(LevelContainer*);
    void Update();
    void Render();

    Entity* Get(int id) const;
    void Spawn(Entity*, float, float);
    ~EntityManager();

private:
    LevelContainer* levelContainer;
    int nextID = 0;
    std::vector<Entity*> entities;
};

class LevelProcessor {
public:
    LevelProcessor(LevelContainer*);
    ~LevelProcessor();
    void Update();

private:
    LevelContainer* levelContainer;
};

class LevelRenderer {
public:
    LevelRenderer(LevelContainer*);
    ~LevelRenderer();
    void Render();

private:
    LevelContainer* levelContainer;
};

class LevelContainer {
public:
    LevelContainer();

    void Update();
    void Render();

    Level* GetLevelData();
    LevelProcessor* GetLevelProcessor();
    LevelRenderer* GetLevelRenderer();
    EntityManager* GetEntityManager();

    ~LevelContainer();

private:
    Level* currentLevel;
    LevelProcessor* lp;
    LevelRenderer* lr;
    EntityManager* em;
};

#endif