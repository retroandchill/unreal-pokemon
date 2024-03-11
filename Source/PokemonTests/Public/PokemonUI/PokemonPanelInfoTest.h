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

public:
	/**
	 * Sets default values for this actor's properties
	 */
	APokemonPanelInfoTest();

private:
	/**
	 * Actually run the test
	 */
	UFUNCTION()
	void RunTest();

	/**
	 * The class used for the screen
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
	TSubclassOf<UPokemonSelectionPane> ScreenClass;
};
