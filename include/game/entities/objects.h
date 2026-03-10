#ifndef ENTITYOBJECTS_H
#define ENTITYOBJECTS_H 1

#include <game/gameprocessor.h>

class Coin : public Entity {
public:
    Coin();
    void Update() override;
    void Tick() override;
    void Render() override;
    ~Coin();

    void OnSpawn() override;
    void OnDeath() override;
};

#endif