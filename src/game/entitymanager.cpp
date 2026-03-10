#include <game/gameprocessor.h>

Entity::Entity(float x, float y, float width, float height) {
    position_size = new GEC::Rect<float,float,float,float>(x,y,width,height);
};
Entity::~Entity() {
    delete position_size;
};

GEC::Vector2<float, float> Entity::Position() const
{
    return this->position_size->First();
}
GEC::Vector2<float, float> Entity::Size() const
{
    return this->position_size->Second();
}

void Entity::Kill() {
    OnDeath();
    isDead = true;
}
bool Entity::IsDead() const {
    return isDead;
}
float Entity::Health() const {
    return health;
}
void Entity::SetHealth(float value) {
    health = value;
}
unsigned int Entity::GetID() const {
    return id;
}
void Entity::SetID(unsigned int id) {
    this->id = id;
}












EntityManager::EntityManager(LevelContainer* lp) {
    this->levelContainer = lp;
}
void EntityManager::Update() {
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        if (!entity->isDead) {
            if (!entity->spawned) {
                entity->spawned = true;
                entity->OnSpawn();
            }
            entity->Update();
        }
        if (entity->Health() <= 0)
            entity->Kill();
        ++i;
    }
}
void EntityManager::Render() {
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        entity->Render();
        ++i;
    }
}
Entity* EntityManager::Get(int id) const {
    int i = 0;
    while (i < entities.size()) {

        ++i;
    }
}
EntityManager::~EntityManager() {
    int i = entities.size()-1;
    while (i >= 0) {
        Entity* entity = entities.at(i);
        delete entity;
        ++i;
    }
    entities.clear();
}
void EntityManager::Spawn(Entity* e, float x, float y) {
    int id = this->nextID;
    e->SetID(id);
    ++id;

    e->position_size->First().Set(x,y);

    entities.push_back(e);


}