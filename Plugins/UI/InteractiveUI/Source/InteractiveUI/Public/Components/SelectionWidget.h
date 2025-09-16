// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SelectionWidget.generated.h"

class UInteractiveButtonGroup;
/**
 * 
 */
UCLASS(Abstract)
class INTERACTIVEUI_API USelectionWidget : public UCommonActivatableWidget
{
    GENERATED_BODY()
protected:
    void NativePreConstruct() override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UInteractiveButtonGroup* GetButtons() const
    {
        return Buttons.Get();
    }

public:
    UWidget* NativeGetDesiredFocusTarget() const override;

    const TOptional<int32>& GetDesiredFocusIndex() const
    {
        return DesiredFocusIndex;
    }
    
    UFUNCTION(BlueprintCallable, Category = "Selection")
    bool TryGetDesiredFocusIndex(int32& OutIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SetDesiredFocusIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ClearDesiredFocusIndex()
    {
        DesiredFocusIndex.Reset();
    }

private:
    UPROPERTY(BlueprintGetter = GetButtons, Category = "Selection")
    TObjectPtr<UInteractiveButtonGroup> Buttons;

    UPROPERTY(EditAnywhere, Getter, Category = "Selection", meta = (ClampMin = 0, UIMin = 0))
    TOptional<int32> DesiredFocusIndex;

};
