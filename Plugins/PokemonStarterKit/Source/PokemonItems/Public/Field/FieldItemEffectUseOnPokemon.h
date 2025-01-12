// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FieldItemEffect.h"
#include "UObject/Object.h"

#include "FieldItemEffectUseOnPokemon.generated.h"

class IPokemon;
struct FItem;

/**
 * Item effect type for items that are used on a Pokémon.
 */
UCLASS(Abstract)
class POKEMONITEMS_API UFieldItemEffectUseOnPokemon : public UFieldItemEffect {
    GENERATED_BODY()

public:
    UE5Coro::TCoroutine<bool> UseItem(const FItem &Item, int32 Quantity, const TScriptInterface<IPokemon> &Pokemon);
    
  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Use(const FItem &Item, int32 Quantity, const TScriptInterface<IPokemon> &Pokemon);
};
