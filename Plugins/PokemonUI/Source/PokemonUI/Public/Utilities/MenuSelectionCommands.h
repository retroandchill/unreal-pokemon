// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Nodes/SelectPokemonFromParty.h"
#include "Screens/PartyScreen.h"

#include "MenuSelectionCommands.generated.h"

struct FItem;

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FItemSelectedDynamic, const TScriptInterface<class IInventoryScreen>&, Screen, const FItem&, Item, int32, Quantity);

/**
 * Blueprint function library of various commands that involve selecting something from a menu.
 */
UCLASS()
class POKEMONUI_API UMenuSelectionCommands : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Select a Pokémon from the player's party and perform a callback after selection
     * @param WorldContext The context used to obtain the world
     * @param HelpText The help text displayed to the player
     * @param Action The action to perform on the Pokémon
     */
    //UFUNCTION(BlueprintCallable, Category = "Events|Selection|Pokémon", meta = (WorldContext = WorldContext))
    static void SelectPokemonFromParty(const UObject* WorldContext, const FText &HelpText, const FPokemonSelectedDynamic& Action);

    /**
     * Select a Pokémon from the player's party and perform a callback after selection
     * @param WorldContext The context used to obtain the world
     * @param HelpText The help text displayed to the player
     * @param Action The action to perform on the Pokémon
     */
    static void SelectPokemonFromParty(const UObject* WorldContext, const FText &HelpText, const FOnPokemonSelected& Action);

    /**
     * Select an item player's inventory and perform a callback after selection
     * @param Action The action to perform on the item
     */
    UFUNCTION(BlueprintCallable, Category = "Events|Selection|Pokémon")
    static void SelectItemFromBag(const FItemSelectedDynamic& Action);

};
