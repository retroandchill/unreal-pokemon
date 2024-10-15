// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Trainers/TrainerGender.h"

#include "PocketWindow.generated.h"

class UCommonTextBlock;
class UPocketGraphic;
class UDisplayText;

/**
 * Window that displays the current pocket to the player.
 */
UCLASS(Abstract)
class POKEMONUI_API UPocketWindow : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Get the current pocket
     * @return The current pocket that is being viewed
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Inventory)
    FName GetCurrentPocket() const;

    /**
     * Callback for when the pocket is set by the selection window.
     * @param Pocket The pocket that we are now pointing to.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Inventory)
    void SetCurrentPocket(FName Pocket);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SetPocketName(const FText& PocketName);

  private:
    /**
     * Update the information displayed to the player regarding the current pocket
     */
    void UpdatePocketInfo();
    
    /**
     * The current pocket that is being viewed
     */
    UPROPERTY(BlueprintGetter = GetCurrentPocket, BlueprintSetter = SetCurrentPocket, Category = Inventory)
    FName CurrentPocket;
};
