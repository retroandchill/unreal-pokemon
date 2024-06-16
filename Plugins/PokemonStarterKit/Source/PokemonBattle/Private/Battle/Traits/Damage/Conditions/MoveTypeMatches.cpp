// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/Damage/Conditions/MoveTypeMatches.h"
#include "Battle/Moves/BattleDamage.h"

bool UMoveTypeMatches::Evaluate_Implementation(const FMoveDamageInfo &Context) const {
    return Context.Type == Type;
}