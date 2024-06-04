// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "ScreenInterface.h"
#include "Blueprint/UserWidget.h"

#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;
/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UScreen : public UUserWidget,
#if CPP
    public virtual IScreenInterface {
#else
    public IScreenInterface {
#endif
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
    
    UFUNCTION(BlueprintCallable, Category = Navigation)
    void CloseScreen() final;

  private:
    /**
     * Get list of selectable widgets in the window
     */
    UPROPERTY()
    TArray<TObjectPtr<USelectableWidget>> SelectableWidgets;
};
