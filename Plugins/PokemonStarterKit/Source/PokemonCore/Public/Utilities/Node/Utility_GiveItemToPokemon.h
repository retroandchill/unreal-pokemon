// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprints/BlueprintUtilityNode.h"
#include "UObject/Object.h"
#include "Utility_GiveItemToPokemon.generated.h"

struct FItemHandle;
class IPokemon;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (UtilityNode))
class POKEMONCORE_API UUtility_GiveItemToPokemon : public UBlueprintUtilityNode {
    GENERATED_BODY()

public:
    bool ImplementsGetWorld() const override;

    void Execute(const FItemHandle &Item, const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex,
        FSimpleDelegate &&ItemGiven, FSimpleDelegate &&ItemNotGiven);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Execute(const FItemHandle &Item, const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemGiven();

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemNotGiven();

private:
    FSimpleMulticastDelegate OnItemGiven;
    FSimpleMulticastDelegate OnItemNotGiven;

};
