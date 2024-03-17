// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "PokemonBuilderTest.generated.h"

/**
 * Test for verify the Pokémon Builder works
 */
UCLASS()
class POKEMONTESTS_API APokemonBuilderTest : public AFunctionalTest {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	APokemonBuilderTest();

private:
	/**
	 * Actually run the test
	 */
	UFUNCTION()
	void RunTest();
	
};
