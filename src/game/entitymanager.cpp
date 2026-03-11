#include <game/gameprocessor.h>

#include <engine/physics/box_collider.h>
#include <game/deltatime.h>
#include <game/tile.h>

Entity::Entity(float x, float y, float width, float height) : friction(10.0f,0), force(0,0)
{
    position_size = new GEC::Rect<float, float, float, float>(x, y, width, height);
};
Entity::~Entity()
{
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

    float nx = position_size->X() + (force.First() * dtime) / mass;
    float ny = position_size->Y() + (force.Second() * dtime) / mass;

    std::vector<bool> collideAxis = ProcessForceCollision(position_size->X(), position_size->Y(), nx, ny, position_size->W(), position_size->H(), this);

    bool collidesX = collideAxis[0];
    bool collidesY = collideAxis[1];
    bool collidesB = collideAxis[2];

    if (collidesX) {
        nx = position_size->X();
        force.First() = 0;
    }
    if (collidesY) {
        ny = position_size->Y();
        force.Second() = 0;
    }
    if (collidesB && (!collidesX && !collidesY)) {
        nx = position_size->X();
        ny = position_size->Y();
        force.First() = 0;
        force.Second() = 0;
    }

    position_size->X() = nx;
    position_size->Y() = ny;

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
                int tileID = l->GetTile(tx, ty, 0);
                Tile* tile = TileList::GetInstance().GetTile(tileID);
                if (tile) {
                    if (tile->SolidTop())
                        if (entity->force.Second() <= 0 && entity->position_size->Y() - entity->position_size->H() > ty) {
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
    return isColliding;
}
void Entity::ProcessGravity()
{
    float dtime = GetMainDeltaTime() / 1000.0f;
    force.Second() -= 9.5 * 15 * dtime;
}
bool Entity::OnSolidGround()
{
    float dSize = .05f;
    return Collides(position_size->X(), position_size->Y() - position_size->H() / 2 - dSize / 2, position_size->W() * .9f, dSize, this);
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
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        if (!entity->isDead) {
            if (!entity->spawned) {
                entity->spawned = true;
                entity->OnSpawn();
            }
            entity->Tick();
        }
        if (entity->Health() <= 0)
            entity->Kill();
        ++i;
    }
}
void EntityManager::Render()
{
    int i = 0;
    while (i < entities.size()) {
        Entity* entity = entities.at(i);
        entity->Render();
        ++i;
    }
}
Entity* EntityManager::Get(int id) const
{
    throw std::runtime_error("EntityManager::Get(int id) has not been declared");
    return nullptr;
}
EntityManager::~EntityManager()
{
    int i = entities.size() - 1;
    while (i >= 0) {
        Entity* entity = entities.at(i);
        delete entity;
        ++i;
    }
    entities.clear();
}
void EntityManager::Spawn(Entity* e, float x, float y)
{
    int id = this->nextID;
    e->id;
    e->manager = this;
    ++id;

    e->position_size->First().Set(x, y);

    entities.push_back(e);
}
LevelContainer* EntityManager::Container() const {
    return this->levelContainer;
}