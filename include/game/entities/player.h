// #############################################################################
// # player.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2026
// # Last Modification:  03/10/2026
// #############################################################################
// # Player Implementation
// #############################################################################

#ifndef PLAYER_H
#define PLAYER_H 1

#include <game/gameprocessor.h>

/**
 * Main Player Obect
 */
class Player : public Entity {
public:
    /**
     * Default Player Constructor
     */
    Player();

    Player(const Player& other)
        : Entity(other)
    {
        jumped = other.jumped;
        isInAJump = other.isInAJump;

        input_left = other.input_left;
        input_right = other.input_right;
        input_x_axis = other.input_x_axis;
        input_jump = other.input_jump;

        _imgX = other._imgX;
        _imgY = other._imgY;
        _dir = other._dir;

        walk_ani = other.walk_ani;
        walk_time = other.walk_time;

        collectedCoins = other.collectedCoins;
        userInput = other.userInput;
    }
    GameObject* Clone() const override
    {
        return new Player(*this);
    }

    /**
     * Update the player, this is mainly inputs
     */
    void Update() override;

    /**
     * Process the player's physics, health, and movement
     */
    void Tick() override;

    /**
     * Draws the player
     */
    void Render() override;

    /**
     * Default Deconstructor for player
     */
    ~Player();

    /** Called on player spawn */
    void OnSpawn() override;

    /** Called on player Deaths */
    void OnDeath() override;

    /** Number of coins the player has collected */
    int CollectedCoins() const
    {
        return collectedCoins;
    }

    /**
     * Allow the player to be moved via user intput
     */
    void EnableInput()
    {
        userInput = true;
    }

    /**
     * Disallow the player to be moved via user intput
     */
    void DisableInput()
    {
        userInput = false;
    }

    /** Allow the coin class to modify the private memebers of this class */
    friend class Coin;

private:
    bool jumped = false, isInAJump = false;
    float input_left, input_right, input_x_axis, input_jump;

    int _imgX = 0, _imgY = 0, _dir = 1;
    float walk_ani = 0, walk_time = 0;

    int collectedCoins = 0;

    bool userInput = true;
};

#endif