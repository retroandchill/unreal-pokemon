// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "NativeGameplayTags.h"
#include "Blueprint/UserWidget.h"

#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenClosed);


namespace RPG::Menus {

/**
 * The native tag assigned the primary layer for display
 */
RPGMENUS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryMenuLayerTag)

/**
 * The native tag assigned to any overlay layers layer for display
 */
RPGMENUS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(OverlayMenuLayerTag)

}

/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class RPGMENUS_API UScreen : public UCommonActivatableWidget {
    GENERATED_BODY()

  public:
    TSharedRef<SWidget> RebuildWidget() override;

    /**
     * Refresh the display of this screen to the player
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Display)
    void RefreshSelf();

    /**
     * Close the screen and return to the previous one
     */
    UFUNCTION(BlueprintCallable, Category = Navigation)
    void CloseScreen();

    /**
     * Callback to bind additional functionality to when CloseScreen() gets called
     * @return Callback for when the screen is closed
     */
    FOnScreenClosed &GetOnScreenClosed();

protected:
    void NativeOnActivated() override;
    void NativeOnDeactivated() override;

  private:
    /**
     * Get list of selectable widgets in the window
     */
    UPROPERTY()
    TArray<TObjectPtr<USelectableWidget>> SelectableWidgets;

    /**
     * Callback for when the screen is closed
     */
    UPROPERTY(BlueprintAssignable)
    FOnScreenClosed OnScreenClosed;
};
