// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"
#include "SelectableOptionTestWidget.generated.h"

class USelectableOption;


UCLASS(Abstract)
class USelectableOptionTestWidget : public USelectableWidget {
    GENERATED_BODY()

public:
    void CreateChildWidgets();

    UFUNCTION(BlueprintImplementableEvent, Category = Components)
    void SlotCreatedWidget(USelectableOption *Option);

    int32 GetItemCount_Implementation() const override;

  const TArray<TObjectPtr<USelectableOption>> &GetOptions() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = Components)
    TSubclassOf<USelectableOption> OptionWidget;

    UPROPERTY()
    TArray<TObjectPtr<USelectableOption>> Options;

};