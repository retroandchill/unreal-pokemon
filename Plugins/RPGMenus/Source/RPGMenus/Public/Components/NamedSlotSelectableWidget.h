// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SelectableWidget.h"

#include "NamedSlotSelectableWidget.generated.h"

/**
 * Special type of selectable widget that utilizes a named slot to house a panel widget which then can also contain
 * additional buttons.
 */
UCLASS(Abstract)
class RPGMENUS_API UNamedSlotSelectableWidget : public USelectableWidget
{
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;

  public:
    UFUNCTION(BlueprintCallable, Category = Selection)
    void UpdateOptions();

  private:
    /**
     * The slot that will contain the window's content
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNamedSlot> ContentSlot;
};
