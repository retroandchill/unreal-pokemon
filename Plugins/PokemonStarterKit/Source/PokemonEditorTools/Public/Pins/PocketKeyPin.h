// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "KismetPins/SGraphPinStructInstance.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class POKEMONEDITORTOOLS_API SPocketKeyPin : public SGraphPinStructInstance {
    SLATE_BEGIN_ARGS(SPocketKeyPin) {
    }

    SLATE_END_ARGS()

    /** Constructs this widget with InArgs
     * @param InArgs The slate arguments
     * @param InGraphPin The pin that we are associated with
     */
    void Construct(const FArguments &InArgs, UEdGraphPin *InGraphPin);

  protected:
    void ParseDefaultValueData() override;
    TSharedRef<SWidget> GetDefaultValueWidget() override;

  private:
    bool RowMatches(const TSharedPtr<FString> &Str) const;
    const TSharedPtr<FString> &GetItemString() const;
    FText GetItemText() const;

    TArray<TSharedPtr<FString>> Options;
    FPocketKey Handle;
};
