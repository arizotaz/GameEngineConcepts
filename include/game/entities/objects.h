// #############################################################################
// # objects.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/16/2026
// # Last Modification:  03/18/2026
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

#include <engine/physics.h>
#include <engine/renderobjects.h>
#include <game/global_states.h>
#include <game/gameprocessor.h>
#include <game/levelcontainer.h>
#include <engine/audio.h>

class Coin : public Entity {
public:
    Coin();
    Coin(const Coin& other)
        : Entity(other)
    {
    }
    GameObject* Clone() const override
    {
        return new Coin(*this);
    }
    void Update() override;
    void Tick() override;
    void Render() override;
    ~Coin();

    void OnSpawn() override;
    void OnDeath() override;
    void WriteObject(std::ostream& out) const override {};
    void ReadObject(std::istream& in) override {}

};

// #############################################################################
// # finish_line.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/18/2026
// # Last Modification:  03/18/2026
// #############################################################################
// # Finish Line
// #############################################################################
// # This object represents the finish line for the game, it is an entity, but
// # interacts with the player and LevelContainer
// #############################################################################
/**
 * Finish Line Object
 */
class FinishLine : public Entity {
public:
    FinishLine();

    FinishLine(const FinishLine& other)
        : Entity(other)
    {
    }
    GameObject* Clone() const override
    {
        return new FinishLine(*this);
    }

    void Update() override;
    void Tick() override;
    void Render() override;
    ~FinishLine();

    void SetLevelContainer(LevelContainer* lc);

    void WriteObject(std::ostream& out) const override;
    void ReadObject(std::istream& in) override;
private:
    LevelContainer* lc;
};

class Melbin : public Entity {
public:
    Melbin();
    Melbin(const Melbin& other)
        : Entity(other)
    {
    }
    GameObject* Clone() const override
    {
        return new Melbin(*this);
    }
    void Update() override;
    void Tick() override;
    void Render() override;
    ~Melbin();

    void OnSpawn() override;
    void OnDeath() override;
    void WriteObject(std::ostream& out) const override {};
    void ReadObject(std::istream& in) override {}

    private:
    bool active = false;
};

#endif