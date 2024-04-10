// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GridBasedGameModeBase.generated.h"

DECLARE_DYNAMIC_DELEGATE(FScreenTransitionCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenTransitionFinished);

/**
 * Game Mode class used to define the information about all of the characters in the game
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGridBasedGameModeBase : public AGameMode {
	GENERATED_BODY()

public:
	/**
	 * Get the size of the grid in the game
	 * @return The size of the grid according to the game
	 */
	UFUNCTION(BlueprintPure, Category = "Map|Grid")
	double GetGridSize() const;
	
	/**
	 * Fade the screen in
	 * @param Callback The delegate to call to when complete
	 */
	UFUNCTION(BlueprintCallable, Category = "Screen Transitions")
	void FadeIn(const FScreenTransitionCallback& Callback);

	/**
	 * Fade the screen in
	 */
	void FadeIn();
	
	/**
	 * Fade the screen out
	 * @param Callback The delegate to call to when complete
	 */
	UFUNCTION(BlueprintCallable, Category = "Screen Transitions")
	void FadeOut(const FScreenTransitionCallback& Callback);

	/**
	 * Fade the screen out
	 */
	void FadeOut();
	
protected:
	/**
	 * Fade the screen in from black
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Screen Transitions")
	void ScreenFadeIn();

	/**
	 * Fade the screen out to black
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Screen Transitions")
	void ScreenFadeOut();

public:
	/**
	 * Delegate called when the screen transition has completed
	 */
	UPROPERTY(BlueprintCallable, Category = "Screen Transition")
	FOnScreenTransitionFinished OnScreenTransitionFinished;

	/**
	 * Override of the default grid size if set.
	 */
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TOptional<double> GridSize;
};
