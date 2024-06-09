// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Effects/BattlerEffect.h"
#include "UObject/Object.h"
#include "DamageEffects.generated.h"

/**
 * The effect granted by the second hit of the ability Parental-Bond.
 */
UCLASS()
class POKEMONBATTLE_API UEffectParentalBond : public UObject, public IBattlerEffect {
    GENERATED_BODY()

public:
    void ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,  int32 BaseDamage, FName MoveType) override;

};

/**
 * The effect granted by the move Me-First.
 */
UCLASS()
class POKEMONBATTLE_API UEffectMeFirst : public UObject, public IBattlerEffect {
    GENERATED_BODY()

public:
    void ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,  int32 BaseDamage, FName MoveType) override;

};

/**
 * The effect granted by the move Helping-Hand.
 */
UCLASS()
class POKEMONBATTLE_API UEffectHelpingHand : public UObject, public IBattlerEffect {
    GENERATED_BODY()

public:
    void ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,  int32 BaseDamage, FName MoveType) override;

};

/**
 * The effect granted by the move Charge.
 */
UCLASS()
class POKEMONBATTLE_API UEffectCharge : public UObject, public IBattlerEffect {
    GENERATED_BODY()

public:
    void ModifyDamageForUser_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,  int32 BaseDamage, FName MoveType) override;

};