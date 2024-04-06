// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "SelectableWindowInputTest.generated.h"

class USelectableWidget;

UCLASS()
class POKEMONTESTS_API ASelectableWindowInputTest : public AFunctionalTest {
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
	 * Callback for the confirm operation
	 * @param Index The index passed through
	 */
	UFUNCTION()
	void OnConfirm(int32 Index);

	/**
	 * Callback for the cancel operation
	 */
	UFUNCTION()
	void OnCancel();

	/**
	 * The widget to create for the test. Assumes a 3x2 grid of inputs for the data to parse correctly
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters")
	TSubclassOf<USelectableWidget> WidgetClass;

	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey UpKey;
	
	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey DownKey;
	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey LeftKey;
	
	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey RightKey;

	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey ConfirmKey;

	/**
	 * The key to press for the up direction
	 */
	UPROPERTY(EditInstanceOnly, Category = "Test Parameters|Inputs")
	FKey CancelKey;

	/**
	 * Used to check if the index is passed through confirm correctly
	 */
	TOptional<int32> NewIndex;

	/**
	 * Used to check if the cancel command is correctly invoked
	 */
	bool bCanceled = false;
};
