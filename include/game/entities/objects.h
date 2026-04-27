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
    FinishLine()
        : Entity("gec.assign2.finish", 0, 0, 6, 0.5)
    {
        this->mass = 3;
        this->name = "FinishLine";
    };

    FinishLine(const FinishLine& other)
        : Entity(other)
    {
    }
    GameObject* Clone() const override
    {
        return new FinishLine(*this);
    }

    void Update() override {

    };
    void Tick() override
    {
        float dtime = GetMainDeltaTime() / 1000.0f;
        ProcessForce();
        ProcessGravity();

        Player* p = lc->GetPlayer();
        GEC::Vector2<float, float>* pos = p->Position();
        GEC::Vector2<float, float> size = p->Size();

        GEC::Physics::BoxCollider2D player_collider(pos->First(), pos->Second(), size.First(), size.Second());
        GEC::Physics::BoxCollider2D my_collider(Position()->First(), Position()->Second(), Size().First() - 1, Size().Second());

        if (my_collider.IsColliding(player_collider)) {
            if (!lc->GameWon()) {
                GEC::AudioEngine::GetInstance().PlaySound("finish_line");
                lc->MarkWin();
            }
        }
    };
    void Render() override
    {
        GEC::Rect<float, float, float, float> r(position->First(), position->Second(), size.First(), size.Second());
        GEC::Render::SetColor(255);
        GEC::Render::Image("game.finish.base", 0, 0 - r.H(), 1.1, r.W(), r.H());
        GEC::Render::Image("game.finish.flag", 0 - r.W() / 2 + (r.W() / 4) / 2, 0 + r.H() * 4 / 2, 1.05, r.W() / 3, r.H() * 6);
    };
    ~FinishLine() { };

    void SetLevelContainer(LevelContainer* lc) { this->lc = lc; }

private:
    LevelContainer* lc;
};

#endif