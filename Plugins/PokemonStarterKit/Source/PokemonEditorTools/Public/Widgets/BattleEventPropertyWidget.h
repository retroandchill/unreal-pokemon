// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class POKEMONEDITORTOOLS_API SBattleEventPropertyWidget : public SCompoundWidget {
public:
    /** Called when a tag status is changed */
    DECLARE_DELEGATE_OneParam(FOnAttributeChanged, FProperty*)
    
    SLATE_BEGIN_ARGS(SBattleEventPropertyWidget) : _DefaultProperty(nullptr)
    {}
    SLATE_ARGUMENT(FProperty*, DefaultProperty)
    SLATE_ARGUMENT(TArray<FProperty*>, AllProperties)
        SLATE_ARGUMENT(UScriptStruct*, StructType)
    SLATE_EVENT(FOnAttributeChanged, OnAttributeChanged) // Called when a tag status changes
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments &InArgs);

private:
    
    TSharedRef<SWidget> GenerateAttributePicker();
    FText GetSelectedValueAsString() const;
    
    /** Handles updates when the selected attribute changes */
    void OnAttributePicked(FProperty* InProperty);

    /** Delegate to call when the selected attribute changes */
    FOnAttributeChanged OnAttributeChanged;

    /** The currently selected attribute */
    FProperty* SelectedProperty = nullptr;

    TObjectPtr<UScriptStruct> StructType;

    /** Used to display an attribute picker. */
    TSharedPtr<SComboButton> ComboButton;
};
