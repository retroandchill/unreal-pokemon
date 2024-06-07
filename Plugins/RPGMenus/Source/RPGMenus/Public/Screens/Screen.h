// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenClosed);

/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class RPGMENUS_API UScreen : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Construct the default version of the screen
     * @param ObjectInitializer The initializer used by Unreal Engine to build the object
     */
    explicit UScreen(const FObjectInitializer &ObjectInitializer);

    TSharedRef<SWidget> RebuildWidget() override;

    /**
     * Grant focus to this menu when the player selects it.
     * @return Was focus granted to a widget.
     */
    virtual bool GiveMenuFocus();

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
