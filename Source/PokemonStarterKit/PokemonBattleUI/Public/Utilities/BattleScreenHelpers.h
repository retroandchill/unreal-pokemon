// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BattleScreenHelpers.generated.h"

class UPokemonBattlePanel;
class UPokemonBattleScreen;
class IBattler;
/**
 * Helper class for working with the battle screen and updating parts of it
 */
UCLASS()
class POKEMONBATTLEUI_API UBattleScreenHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Find the battle screen given the provided world context object
     * @param WorldContextObject The object used to obtain the world
     * @return The battle screen (or nullptr if not found)
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    static UPokemonBattleScreen *FindBattleScreen(const UObject *WorldContextObject);

    /**
     * Find the battle pannel for the given battler to update as needed
     * @param WorldContextObject The object used to obtain the world
     * @param Battler The battler to get the panel for
     * @return The battler's information panel (or nullptr if not found)
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    static UPokemonBattlePanel *FindPokemonBattlePanel(const UObject *WorldContextObject,
                                                       const TScriptInterface<IBattler> &Battler);

    /**
     * Refresh the displayed status condition of the given battler
     * @param WorldContextObject The context used to obtain information about the world
     * @param Battler The battler to update the status for
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display", meta = (WorldContext = "WorldContextObject"))
    static void RefreshDisplayedStatusCondition(const UObject *WorldContextObject,
                                                const TScriptInterface<IBattler> &Battler);
};
