// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/Moves/MoveTypeDeterminedPayload.h"

const TScriptInterface<IBattler> &UMoveTypeDeterminedPayload::GetUser() const
{
    return User;
}