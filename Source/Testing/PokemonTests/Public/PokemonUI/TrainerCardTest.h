// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "TrainerCardTest.generated.h"

class UTrainerCardScreen;

UCLASS()
class POKEMONTESTS_API ATrainerCardTest : public AFunctionalTest {
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
	 * The widget to create for the test
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
	TSubclassOf<UTrainerCardScreen> WidgetClass;
};
