// #############################################################################
// # gameProcessors.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/03/2026
// # Last Modification:  03/18/2026
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

#include <engine/game_structs.h>
#include <engine/structs.h>

#include <game/level.h>

#include <engine/ui/elements.h>

class LevelContainer;
class EntityManager;

class Entity : public GEC::Game::GameObject {
public:
    Entity(const char* id, float, float, float, float);
    Entity(const Entity& other);
    virtual GameObject* Clone() const override = 0;

    virtual void OnSpawn() { }
    virtual void Start() override { }
    virtual void Tick() = 0;
    virtual void Update() override = 0;
    virtual void Render() override = 0;
    virtual void OnDeath() { };
    virtual void DeathLoop() { releaseEntity = true; }
    virtual void OnRemove() { }

    virtual ~Entity();
    GEC::Vector2<float, float>* Position() const;
    GEC::Vector2<float, float> Size() const;
    GEC::Vector2<float, float> RawSize() const;

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


    void UpdatePropertiesPanel(float x, float y, float width, float height) override
    {
        if (useGravityCheckbox == nullptr) {
            useGravityCheckbox = new GEC::UI::Elements::Checkbox();
            useGravityCheckbox->SetValue(this->useGravity);
        }

        float bSize = 20;
        useGravityCheckbox->Set("Process Gravity",x-width/2,y+height/2-bSize/2,width,bSize);
        useGravityCheckbox->Update();
    }
    void InteractPropertiesPanel(float x, float y, float width, float height) override {
        useGravityCheckbox->Interact();
        if (useGravityCheckbox->Changed()) {
            this->useGravity = useGravityCheckbox->GetValue();
        }
    }
    void RenderPropertiesPanel(float x, float y, float width, float height) override {
        if (useGravityCheckbox!= nullptr)useGravityCheckbox->Render();
    }

protected:
    unsigned int id;
    const char* typeID;

        GEC::Vector2<float, float> size;

    bool useGravity = true;
    float health = 1;
    bool isDead = false, releaseEntity = false;
    bool spawned = false;
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


    GEC::UI::Elements::Checkbox* useGravityCheckbox = nullptr;
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
    LevelRenderer(Level*);
    ~LevelRenderer();
    void Render();
    void DrawLayer(int layer, int z);
    LevelContainer* Container() const;
    Level* GetLevel() const;

private:
    LevelContainer* levelContainer = nullptr;
    Level* level = nullptr;
};

#endif