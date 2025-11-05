// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "Lookup/InjectableDependency.h"
#include "NativeGameplayTags.h"

#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;

DECLARE_MULTICAST_DELEGATE(FOnScreenClosed);

namespace RPG::Menus {

    /**
     * The native tag assigned the primary layer for display
     */
    RPGMENUS_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryMenuLayerTag);

    /**
     * The native tag assigned to any overlay layers layer for display
     */
    RPGMENUS_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(OverlayMenuLayerTag);

} // namespace RPG::Menus

UENUM(BlueprintType)
enum class ERPGWidgetInputMode : uint8 { Default, GameAndMenu, Game, Menu };

/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class RPGMENUS_API UScreen : public UCommonActivatableWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

    UWidget *NativeGetDesiredFocusTarget() const override;

  public:
    /**
     * Refresh the display of this screen to the player
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Display)
    void RefreshSelf();

    TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

    /**
     * Close the screen and return to the previous one
     */
    UFUNCTION(BlueprintCallable, Category = Navigation)
    virtual void CloseScreen();

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
    FOnScreenClosed OnScreenClosed;

    /**
     * The desired input mode to use while this UI is activated, for example do you want key presses to still reach
     * the game/player controller?
     */
    UPROPERTY(EditDefaultsOnly, Category = Input)
    ERPGWidgetInputMode InputConfig = ERPGWidgetInputMode::Default;

    /**
     * The desired mouse behavior when the game gets input.

     */
    UPROPERTY(EditDefaultsOnly, Category = Input)
    EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};

namespace RPG::Menus {
    template <typename T>
    concept InjectableScreen = std::derived_from<T, UScreen> && UnrealInjector::Injectable<T>;

    template <typename T, typename... A>
    concept CanInjectScreen = InjectableScreen<T> && UnrealInjector::CanInitialize<T, A...>;
} // namespace RPG::Menus