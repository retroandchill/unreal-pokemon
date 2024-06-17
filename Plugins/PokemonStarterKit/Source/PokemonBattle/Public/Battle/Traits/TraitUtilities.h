// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"

namespace Traits {

/**
 * Apply the individual damage modifiers for a move with the given context information
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
POKEMONBATTLE_API void ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers,
                                                          const FMoveDamageInfo &Context);

/**
 * Apply the individual critical hit rate modifiers for a move with the given context information
 * @param Stages The number of stages to modify the hit rate by
 * @param Override The current override state
 * @param User The user of the move
 * @param Target The target of the move
 */
POKEMONBATTLE_API void ApplyIndividualCriticalHitRateModifications(int32 &Stages, ECriticalOverride &Override,
                                                                    const TScriptInterface<IBattler> &User,
                                                                    const TScriptInterface<IBattler> &Target);

} // namespace Traits