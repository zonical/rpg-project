#include "rpg/entity.h"
#include "rpg/entities/character.h"
#include "SDL/SDL.h"
#ifndef CHAR_H_GAMESTATE_LOGIC
#define CHAR_H_GAMESTATE_LOGIC

class GameStateLogic
{
private:
    // Private character object that we own and will manage here.
    // The Player class will have access to this as we'll assign it
    // to them as their pawn.
    Character* character = NULL;
public:
    // Initalizer.
    GameStateLogic();
    void InitalizeLoading();
    void InitalizeOverworld();
    void InitalizeBattle();
    void InitalizeMainMenu();
    void InitalizePaused();
    void Update(float);
};

#endif