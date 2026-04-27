#include <engine/physics.h>
#include <game/gameprocessor.h>
#include <game/levelcontainer.h>
#include <game/tile.h>
#include <game/global_states.h>

Entity::Entity(const char* type, float x, float y, float width, float height)
    : GEC::Game::GameObject("com.arizotaz.entity")
    , friction(10.0f, 0)
    , force(0, 0)
    , typeID(type)
    , size(width, height)
{
    this->position->Set(x, y);
};
Entity::Entity(const Entity& other)
    : GEC::Game::GameObject(other)
    , friction(10.0f, 0)
    , force(0, 0)
    , typeID(other.typeID)
    , size(0, 0)
{
    id = other.id;
    typeID = other.typeID;

    size = other.size;
    health = other.health;
    isDead = other.isDead;
    releaseEntity = other.releaseEntity;
    spawned = other.spawned;

    force = other.force;
    friction = other.friction;
    mass = other.mass;

    useGravity = other.useGravity;

    manager = nullptr;
}
Entity::~Entity() { };
GEC::Vector2<float, float>* Entity::Position() const
{
    return position;
}
GEC::Vector2<float, float> Entity::Size() const
{
    return GEC::Vector2<float, float>(size.First()*scale->First(),size.Second()*scale->Second());
}
GEC::Vector2<float, float> Entity::RawSize() const
{
    return GEC::Vector2<float, float>(size.First(),size.Second());
}

void Entity::Kill()
{
    OnDeath();
    isDead = true;
}
bool Entity::IsDead() const
{
    return isDead;
}
float Entity::Health() const
{
    return health;
}
void Entity::SetHealth(float value)
{
    health = value;
}
unsigned int Entity::GetID() const
{
    return id;
}
void Entity::SetID(unsigned int id)
{
    this->id = id;
}
const char* Entity::GetType() const
{
    return this->typeID;
}

std::vector<bool> Entity::ProcessForceCollision(float x, float y, float nx, float ny, float colliderW, float colliderH, Entity* entity)
{
    return std::vector<bool> {
        Collides(nx, y, colliderW, colliderH, this),
        Collides(x, ny, colliderW, colliderH, this),
        Collides(nx, ny, colliderW, colliderH, this)
    };
}
std::vector<bool> Entity::ProcessForce()
{
    float dtime = GetMainDeltaTime() / 1000.0f;

    EntityManager* em = Manager();

    float nx = position->First() + (force.First() * dtime) / mass;
    float ny = position->Second() + (force.Second() * dtime) / mass;

    std::vector<bool> collideAxis = ProcessForceCollision(position->First(), position->Second(), nx, ny, Size().First(), Size().Second(), this);

    bool collidesX = collideAxis[0];
    bool collidesY = collideAxis[1];
    bool collidesB = collideAxis[2];

    if (collidesX) {
        nx = position->First();
        force.First() = 0;
    }
    if (collidesY) {
        ny = position->Second();
        force.Second() = 0;
    }
    if (collidesB && (!collidesX && !collidesY)) {
        nx = position->First();
        ny = position->Second();
        force.First() = 0;
        force.Second() = 0;
    }

    position->Set(nx, ny);

    force.First() -= (friction.First() * force.First()) * dtime;
    force.Second() -= (friction.Second() * force.Second()) * dtime;

    return collideAxis;
}
bool Entity::Collides(float rx, float ry, float rw, float rh, Entity* entity)
{
    EntityManager* em = Manager();
    Level* l = em->Container()->GetLevelData();

    bool isColliding = false;
    GEC::Physics::BoxCollider2D collider = GEC::Physics::BoxCollider2D(rx, ry, rw, rh);
    int px = round(rx), py = round(ry);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            int tx = px + x, ty = py + y;
            if (tx >= 0 && tx < l->Width() && ty >= 0 && ty < l->Height()) {

                for (int ll = 0; ll < l->Layers(); ++ll) {
                    Level_Layer* ld = l->GetLayerData(ll);
                    if (ld->collidable) {
                        int tileID = l->GetTile(tx, ty, ll);
                        Tile* tile = TileList::GetInstance().GetTile(tileID);
                        if (tile) {
                            if (tile->SolidTop())
                                if (entity->force.Second() <= 0 && entity->position->Second() - entity->Size().Second() > ty) {
                                    GEC::Physics::BoxCollider2D tcollider(tx, ty, 1, 1);
                                    if (tcollider.IsColliding(collider)) {
                                        isColliding = true;
                                    }
                                }
                            if (tile->Solid()) {
                                GEC::Physics::BoxCollider2D tcollider(tx, ty, 1, 1);
                                if (tcollider.IsColliding(collider))
                                    isColliding = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return isColliding;
}
void Entity::ProcessGravity()
{
    if (useGravity) {
        float dtime = GetMainDeltaTime() / 1000.0f;
        force.Second() -= 9.5 * 15 * dtime;
    }
}
bool Entity::OnSolidGround()
{
    float dSize = .05f;
    return Collides(position->First(), position->Second() - Size().Second() / 2 - dSize / 2, Size().First() * .9f, dSize, this);
}

EntityManager::EntityManager(LevelContainer* lp)
{
    this->levelContainer = lp;
}
void EntityManager::Update()
{
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        if (!entity->isDead) {
            entity->Update();
        }
        ++i;
    }
}
void EntityManager::Tick()
{
    std::vector<int> removeIDs;
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        if (!entity->isDead) {
            if (!entity->spawned) {
                entity->spawned = true;
                entity->OnSpawn();
            }
            entity->Tick();
            if (entity->Health() <= 0)
                entity->Kill();
        } else {
            if (!entity->releaseEntity) {
                entity->DeathLoop();
            } else {
                removeIDs.push_back(entity->id);
            }
        }
        ++i;
    }

    for (int i = 0; i < removeIDs.size(); ++i) {
        int id = removeIDs[i];
        Entity* e_obj = this->Get(id);
        if (e_obj) {
            auto begin = std::find(entities.begin(), entities.end(), this->Get(id));
            entities.erase(begin);
            delete e_obj;
        }
    }
}
void EntityManager::Render()
{
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        entity->Draw();
        ++i;
    }
}
Entity* EntityManager::Get(int id) const
{
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        if (entity->GetID() == id) {
            i = entities.size();
            return entity;
        }
        ++i;
    }

    return nullptr;
}
std::vector<int> EntityManager::List() const
{
    std::vector<int> v;
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        v.push_back(entity->GetID());
        ++i;
    }
    return v;
}
EntityManager::~EntityManager()
{
    int i = entities.size() - 1;
    while (i >= 0) {
        Entity* entity = entities.at(i);
        if (entity != nullptr)
            delete entity;
        --i;
    }
    entities.clear();
}
void EntityManager::Spawn(Entity* e, float x, float y)
{
    int id = this->nextID;
    ++this->nextID;

    e->id = id;
    e->manager = this;
    ++id;

    e->Position()->Set(x, y);

    entities.push_back(e);
}
LevelContainer* EntityManager::Container() const
{
    return this->levelContainer;
}