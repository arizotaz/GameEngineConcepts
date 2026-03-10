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
    

    Player();
    void Update() override;
    void Tick() override;
    void Render() override;
    ~Player();

    void OnSpawn() override;
    void OnDeath() override;

    private:

    	bool jumped = false, isInAJump = false;
    float input_left, input_right, input_x_axis, input_jump;

    int _imgX = 0, _imgY = 0,_dir = 1;
    float walk_ani = 0, walk_time = 0;
    
};


#endif