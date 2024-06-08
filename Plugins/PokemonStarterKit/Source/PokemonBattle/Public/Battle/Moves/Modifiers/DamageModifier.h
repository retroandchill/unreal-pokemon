// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

struct FBattleDamage;
class IBattler;
class IBattleMove;
class UBaseBattleMove;
struct FModifiedDamage;

namespace Battle::Moves {
/**
 * Represents a low-level damage modifier used by moves
 */
class POKEMONBATTLE_API IDamageModifier {
public:
    virtual ~IDamageModifier() = default;

    /**
     * Apply this damage modifier to a move
     * @param Damage The current damage pre-modification
     * @param Move The move currently being used by the user
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The total number of targets in question
     */
    virtual void ApplyModifier(FModifiedDamage& Damage, const UBaseBattleMove* Move, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount) const = 0;
};
}