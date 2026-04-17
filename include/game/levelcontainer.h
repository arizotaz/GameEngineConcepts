#ifndef GAME_LEVELCONTAINER_H
#define GAME_LEVELCONTAINER_H 1

#include <engine/structs.h>
#include <game/entities/player.h>

class LevelContainer {
public:
    LevelContainer();

    void Update();
    void Tick();
    void Render();

    void SetLevel(Level* level);
    Level* GetLevelData();
    LevelProcessor* GetLevelProcessor();
    LevelRenderer* GetLevelRenderer();
    EntityManager* GetEntityManager();
    GEC::MenuManager* GetMenuManager();

    void SetPlayer(Player* p) { mainPlayer = p; }
    Player* GetPlayer() const { return mainPlayer; }

    void MarkWin() {
        GetPlayer()->DisableInput();
        win = true;
    }
    bool GameWon() const {
        return win;
    }

    bool Paused() const { return paused; }
    void Pause()
    {
        if (!Paused())
            TogglePause();
    }
    void UnPause()
    {
        if (Paused())
            TogglePause();
    }
    void TogglePause() { paused = !paused; }
    float TimeRemaining() const
    {
        return timeRemaining;
    }
    int Coins() const
    {
        return mainPlayer->CollectedCoins();
    }

    ~LevelContainer();

private:
    GEC::MenuManager* lc_mm;
    Level* currentLevel;
    LevelProcessor* lp;
    LevelRenderer* lr;
    EntityManager* em;

    Player* mainPlayer = nullptr;

    float timeRemaining = 0;
    bool paused = false;
    bool win = false;
};

#endif