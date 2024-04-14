﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PokemonTestUtilities.generated.h"

class UWidget;
/**
 * Blueprint function library used for generating mock data for testing purposes
 */
UCLASS()
class POKEMONTESTS_API UPokemonTestUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Create a mock party for the testing purposes. The party in question contains Pokémon with the following stats
     * <p>Samurott (M) @ Life Orb</p>
     * <p>Ability: Torrent</p>
     * <p>Tera Type: Ground</p>
     * <p>EVs: 164 Atk / 92 SpA / 252 Spe</p>
     * <p>Naive Nature</p>
     * <p>- Hydro Pump</p>
     * <p>- Knock Off</p>
     * <p>- Megahorn</p>
     * <p>- Sacred Sword</p>
     * <p></p>
     * <p>Emboar (F) @ Choice Band</p>
     * <p>Ability: Reckless</p>
     * <p>Shiny: Yes</p>
     * <p>Tera Type: Fire</p>
     * <p>EVs: 252 Atk / 4 Def / 252 Spe</p>
     * <p>Jolly Nature</p>
     * <p>- Flare Blitz</p>
     * <p>- Superpower</p>
     * <p>- Wild Charge</p>
     * <p>- Earthquake</p>
     * <p></p>
     * <p>Serperior (M) @ Rocky Helmet</p>
     * <p>Ability: Contrary</p>
     * <p>Tera Type: Grass</p>
     * <p>EVs: 208 HP / 48 Def / 252 Spe</p>
     * <p>Timid Nature</p>
     * <p>IVs: 0 Atk</p>
     * <p>- Leaf Storm</p>
     * <p>- Taunt</p>
     * <p>- Glare</p>
     * <p>- Defog  </p>
     *
     * @param WorldContext The object used to obtain the world context
     */
    UFUNCTION(BlueprintCallable, Category = "Testing|Pokémon", meta = (WorldContext = "WorldContext"))
    static void CreateMockParty(UObject *WorldContext);
};

/**
 * "Deleter" to shutdown the game instance so it doesn't cause issues when it gets garbage collected.
 */
struct FGameInstanceShutdown {
    void operator()(UGameInstance *GameInstance) {
        if (GameInstance != nullptr)
            GameInstance->Shutdown();
    }
};

/**
 * An RAII container for a Game Instance that calls shutdown when the object leaves scope.
 */
using FGameInstancePtr = TUniquePtr<UGameInstance, FGameInstanceShutdown>;

/**
 * "Deleter" to remove a widget from the viewport.
 */
struct FRemoveWidgetFromViewport {
    void operator()(UWidget *Widget) {
        if (Widget != nullptr)
            Widget->RemoveFromParent();
    }
};

/**
 * An RAII container for a Widget that removes it from the viewport when we're done with it.
 */
template <typename T>
    requires std::is_base_of_v<UWidget, T>
using TWidgetPtr = TUniquePtr<T, FRemoveWidgetFromViewport>;