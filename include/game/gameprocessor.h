// #############################################################################
// # gameProcessors.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/03/2025
// # Last Modification:  03/18/2025
// #############################################################################
// # 
// #############################################################################
// #
// # 
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
    Entity(const char* id, float, float, float, float);
    virtual void OnSpawn() { }
    virtual void Tick() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void OnDeath() {};
    virtual void DeathLoop() { releaseEntity = true; }
    virtual void OnRemove() { }

    virtual ~Entity();
    GEC::Vector2<float, float> Position() const;
    GEC::Vector2<float, float> Size() const;

    void Kill();
    bool IsDead() const;
    float Health() const;
    void SetHealth(float);

    unsigned int GetID() const;
    const char* GetType() const;

    virtual EntityManager* Manager() const final
    {
        return manager;
    }

    friend EntityManager;

protected:
    unsigned int id;
    const char* typeID;

    float health = 1;
    bool isDead = false, releaseEntity = false;
    bool spawned = false;
    GEC::Rect<float, float, float, float>* position_size;
    GEC::Vector2<float, float> force;
    GEC::Vector2<float, float> friction;
    float mass = 10;

    std::vector<bool> ProcessForceCollision(float x, float y, float nx, float ny, float colliderW, float colliderH, Entity* entity);
    std::vector<bool> ProcessForce();
    bool Collides(float rx, float ry, float rw, float rh, Entity* entity);
    void ProcessGravity();
    bool OnSolidGround();

private:
    void SetID(unsigned int);
    EntityManager* manager;
};

class EntityManager {
public:
    EntityManager(LevelContainer*);
    void Update();
    void Tick();
    void Render();
    LevelContainer* Container() const;
    Entity* Get(int id) const;

    std::vector<int> List() const;

    void Spawn(Entity*, float, float);
    ~EntityManager();

private:
    LevelContainer* levelContainer;
    unsigned int nextID = 0;
    std::vector<Entity*> entities;
};

class LevelProcessor {
public:
    LevelProcessor(LevelContainer*);
    ~LevelProcessor();
    void Update();
    void Tick();

private:
    LevelContainer* levelContainer;
};

class LevelRenderer {
public:
    LevelRenderer(LevelContainer*);
    ~LevelRenderer();
    void Render();
    void DrawLayer(int layer, int z);
    LevelContainer* Container() const;

private:
    LevelContainer* levelContainer;
};

#endif