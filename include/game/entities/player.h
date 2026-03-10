// #############################################################################
// # player.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2025
// # Last Modification:  03/03/2025
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
    void Render() override;
    ~Player();

    void OnSpawn() override;
    void OnDeath() override;

    private:

    int _dir = 1;
    int _imgX = 0, _imgY = 0;

    float _lastAniX = 0;
    
};


#endif