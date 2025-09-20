// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "GridBasedGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenTransitionFinished);

/**
 * Game Mode class used to define the information about all of the characters in the game
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGridBasedGameModeBase : public AGameMode
{
    GENERATED_BODY()

  public:
    /**
     * Get the size of the grid in the game
     * @return The size of the grid according to the game
     */
    UFUNCTION(BlueprintPure, Category = "Map|Grid")
    double GetGridSize() const;

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

    FDelegateHandle BindToOnScreenTransitionFinished(FSimpleDelegate Delegate)
    {
        return OnScreenTransitionFinished.Add(MoveTemp(Delegate));   
    }

    void UnbindFromOnScreenTransitionFinished(const FDelegateHandle Handle)
    {
        OnScreenTransitionFinished.Remove(Handle);   
    }

protected:
    AActor *ChoosePlayerStart_Implementation(AController *Player) override;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Screen Transitions")
    void ScreenTransitionFinished() const;

  private:
    /**
     * Delegate called when the screen transition has completed
     */
    FSimpleMulticastDelegate OnScreenTransitionFinished;

    /**
     * Override of the default grid size if set.
     */
    UPROPERTY(EditAnywhere, Category = "Tiles")
    TOptional<double> GridSize;
};
