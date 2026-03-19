// #############################################################################
// # objects.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/16/2025
// # Last Modification:  03/18/2025
// #############################################################################
// # Entity Objects Implemntation
// #############################################################################
// # This header contains declarations for each type of entity that represents
// # an object rather than a playable or interactable character.  These are
// # objects like coins, gems, furniture, trees, or any other object that would
// # not attack the player or move on it's own.
// #############################################################################
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