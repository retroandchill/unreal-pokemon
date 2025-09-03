// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Status.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "StatusEffectUtilities.generated.h"

class IPokemon;

/**
 * Enum for determining if a Pokémon has a status effect or not.
 */
UENUM(BlueprintType)
enum class EStatusEffectStatus : uint8 {
    /**
     * The Pokémon has a status effect
     */
    HasStatusEffect,

    /**
     * The Pokémon does not have a status effect
     */
    NoStatusEffect
};

/**
 * Utilities related to Pokémon status effects.
 */
UCLASS()
class POKEMONCORE_API UStatusEffectUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the Pokémon's status effect
     * @param Pokemon The Pokémon to get the status effect for
     * @param StatusEffect The status effect information to write that data into
     * @return If the Pokémon has a status effect
     */
    UFUNCTION(BlueprintCallable, Category = "StatusEffects",
              meta = (ExpandEnumAsExecs = "ReturnValue", DefaultToSelf = "Pokemon"))
    static EStatusEffectStatus GetStatusEffect(const TScriptInterface<IPokemon> &Pokemon, FStatus &StatusEffect);
};
