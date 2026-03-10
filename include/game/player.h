// #############################################################################
// # player.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2025
// # Last Modification:  02/11/2025
// #############################################################################
// # Player Implementation
// #############################################################################

#ifndef PLAYER_H
#define PLAYER_H

/**
 * Main Player Obect
 */
class Player {
    public:
    
    Player(int,int);

    /** Main Logic */
    void Update();

    /** Render */
    void Render();

    ~Player();

    private:
    float _x, _y;
    float _w,_h;

    int _dir = 1;
    int _imgX = 0, _imgY = 0;

    float _lastAniX = 0;
    
};


#endif