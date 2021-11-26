#ifndef PLAYER_H
#define PLAYER_H
#include "rpg/entity.h"

class Player
{
private:
    uint8_t states;

    // This is the entity we're actively controlling at the moment and
    // sending all of our inputs to.
    Entity* pawn;

    bool hasPawn = false;
public:
    // Should we be doing ANYTHING with our pawns?
    // NOTE: This is not the same as checking for PLAYER_IDLE.
    bool playerIsActive;

    // Changing our pawn.
    void SetPlayerPawn(Entity*);
    Entity* Pawn() { return pawn; };
};
#endif