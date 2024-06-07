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
    /**
     * Give an item to a Pokémon
     * @param WorldContext The object used to get the world state
     * @param Item The item being given
     * @param Pokemon The Pokémon the item is being given to
     * @param PokemonIndex The index of the Pokémon in question
     * @param ItemGiven The action taken if an item is given
     * @param ItemNotGiven The action taken if an item is not given
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Utilities|Items")
    void GiveItemToPokemon(const UObject *WorldContext, FName Item, const TScriptInterface<IPokemon> &Pokemon,
                           int32 PokemonIndex, const FItemResultNoRetValue &ItemGiven,
                           const FItemResultNoRetValue &ItemNotGiven);

    /**
     * Take a Pokémon's hold item and return it to the bag
     * @param WorldContext The object used to get the world state
     * @param Pokemon The Pokémon the item is being taken from
     * @param ItemTaken The action taken if an item is taken
     * @param ItemNotTaken The action taken if an item not taken
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Utilities|Items")
    void TakeItemFromPokemon(const UObject *WorldContext, const TScriptInterface<IPokemon> &Pokemon,
        const FItemResultNoRetValue &ItemTaken, const FItemResultNoRetValue &ItemNotTaken);
};