// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utility_GiveItemToPokemon.generated.h"

class IPokemon;

DECLARE_MULTICAST_DELEGATE(FItemResult);

/**
 * 
 */
UCLASS(meta = (UtilityNode))
class POKEMONCORE_API UUtility_GiveItemToPokemon : public UObject {
    GENERATED_BODY()

public:
    bool ImplementsGetWorld() const override;

    void Execute(FName Item, const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex,
        FItemResult::FDelegate &&ItemGiven, FItemResult::FDelegate &&ItemNotGiven);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Execute(FName Item, const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemGiven() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemNotGiven() const;

private:
    FItemResult OnItemGiven;
    FItemResult OnItemNotGiven;

};
