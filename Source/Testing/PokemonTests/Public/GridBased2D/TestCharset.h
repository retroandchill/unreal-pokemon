// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TestCharset.generated.h"

class UCharset;
/**
 * Test class for a charset
 */
UCLASS()
class POKEMONTESTS_API ATestCharset : public AFunctionalTest {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	ATestCharset();

private:
	/**
	 * Actually run the test
	 */
	UFUNCTION()
	void RunTest();

	/**
	 * The Windowskin asset to verify what to do when there is a valid texture
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
	TObjectPtr<UCharset> Charset;
};
