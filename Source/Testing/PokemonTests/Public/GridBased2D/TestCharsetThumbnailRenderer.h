// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TestCharsetThumbnailRenderer.generated.h"

class UCharset;

UCLASS()
class POKEMONTESTS_API ATestCharsetThumbnailRenderer : public AFunctionalTest {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	ATestCharsetThumbnailRenderer();


private:
	/**
	 * Actually run the test
	 */
	UFUNCTION()
	void RunTest();

	/**
	 * The Charset asset to verify what to do when there is a valid texture
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
	TObjectPtr<UCharset> ValidCharset;
};
