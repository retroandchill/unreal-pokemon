// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PokemonPanelInfoTest.generated.h"

class UPokemonSelectionPane;

/**
 * Test to make sure the panels gets populated correctly
 */
UCLASS()
class POKEMONTESTS_API APokemonPanelInfoTest : public AFunctionalTest {
    GENERATED_BODY()

  protected:
    void BeginPlay() override;

  private:
    /**
     * Actually run the test
     */
    UFUNCTION()
    void TestRun();

    /**
     * The class used for the screen
     */
    UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
    TSubclassOf<UPokemonSelectionPane> ScreenClass;
};
