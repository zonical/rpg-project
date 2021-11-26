#include "rpg/player.h"
#include "rpg/entity.h"

void Player::SetPlayerPawn(Entity* ent)
{
    // Take our current pawn (if we have one), and remove it's tag
    // saying it has control of the player.
    if (pawn)
    {
        pawn->RemoveTag(Tag_PlayerEntity);
        pawn->RemoveTag(Tag_Global);
    }

    // Set the entity that we're controlling.
    pawn = ent;
    pawn->AddTag(Tag_PlayerEntity);

    // The player is a global entity. It should not be destroyed between
    // level transitions and only on game session end.
    pawn->AddTag(Tag_Global);
}