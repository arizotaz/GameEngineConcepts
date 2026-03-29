// #############################################################################
// # player.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2025
// # Last Modification:  03/10/2025
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