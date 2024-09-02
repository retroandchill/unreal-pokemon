// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/DataStructHandle.h"
#include "KismetPins/SGraphPinStructInstance.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Widget that represents a data handle struct pin, giving the user a dropdown instead of a regular struct pin.
 */
class POKEMONEDITORTOOLS_API SDataHandlePinStructPin : public SGraphPinStructInstance {
public:
    SLATE_BEGIN_ARGS(SDataHandlePinStructPin) {
        }

    SLATE_END_ARGS()

    /** Constructs this widget with InArgs
     * @param InArgs The slate arguments
     * @param InGraphPin The pin that we are associated with
     */
    void Construct(const FArguments &InArgs, UEdGraphPin* InGraphPin);

protected:
    TSharedRef<SWidget> GetDefaultValueWidget() override;

private:
    bool RowMatches(const TSharedPtr<FString>& Str) const;
    const TSharedPtr<FString> &GetItemString() const;
    FText GetItemText() const;
    
    TArray<TSharedPtr<FString>> Options;
    Pokemon::Data::FStructWrapper Handle;
};
