// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprints/BlueprintUtilityNode.h"
#include "Lookup/InjectableDependency.h"

#include "Utility_TakeItemFromPokemon.generated.h"

class IPokemon;
/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (UtilityNode))
class POKEMONCORE_API UUtility_TakeItemFromPokemon : public UBlueprintUtilityNode {
    GENERATED_BODY()

  public:
    void Execute(const TScriptInterface<IPokemon> &Pokemon, FSimpleDelegate &&ItemTaken,
                 FSimpleDelegate &&ItemNotTaken);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Execute(const TScriptInterface<IPokemon> &Pokemon);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemTaken();

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteItemNotTaken();

  private:
    FSimpleMulticastDelegate OnItemTaken;
    FSimpleMulticastDelegate OnItemNotTaken;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, UUtility_TakeItemFromPokemon);