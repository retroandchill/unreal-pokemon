// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleSide.generated.h"

class UBattleEffect;
class IBattler;
// This class does not need to be modified.
UINTERFACE()
class POKEMONBATTLE_API UBattleSide : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents a "side" of the battle.
 */
class POKEMONBATTLE_API IBattleSide {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual const TArray<TScriptInterface<IBattler>>& GetBattlers() const = 0;

    virtual const TArray<TObjectPtr<UBattleEffect>> &GetBattleEffects() const = 0;

};