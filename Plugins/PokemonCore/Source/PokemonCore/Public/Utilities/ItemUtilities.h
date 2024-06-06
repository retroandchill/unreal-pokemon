// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ItemUtilities.generated.h"

DECLARE_DYNAMIC_DELEGATE(FItemResultNoRetValue);

class IPokemon;
// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UItemUtilities : public UInterface {
    GENERATED_BODY()
};

/**
 *
 */
class POKEMONCORE_API IItemUtilities {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Utilities|Items")
    void GiveItemToPokemon(const UObject *WorldContext, FName Item, const TScriptInterface<IPokemon> &Pokemon,
                           int32 PokemonIndex, const FItemResultNoRetValue &ItemGiven,
                           const FItemResultNoRetValue &ItemNotGiven);
};