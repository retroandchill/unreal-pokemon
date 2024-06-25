// "Unreal Pokémon" created by Retro & Chill.


#include "Widgets/BattleEventPropertyWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/AttributeListWidget.h"

DECLARE_DELEGATE_OneParam(FOnAttributePicked, FProperty*);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SBattleEventPropertyWidget::Construct(const FArguments &InArgs) {
    OnAttributeChanged = InArgs._OnAttributeChanged;
    SelectedProperty = InArgs._DefaultProperty;
    StructType = InArgs._StructType;

    // set up the combo button
    SAssignNew(ComboButton, SComboButton)
        .OnGetMenuContent(this, &SBattleEventPropertyWidget::GenerateAttributePicker)
        .ContentPadding(FMargin(2.0f, 2.0f))
        .ToolTipText(this, &SBattleEventPropertyWidget::GetSelectedValueAsString)
        .ButtonContent()
        [
            SNew(STextBlock)
            .Text(this, &SBattleEventPropertyWidget::GetSelectedValueAsString)
        ];

    ChildSlot
    [
        ComboButton.ToSharedRef()
    ];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SBattleEventPropertyWidget::GenerateAttributePicker() {
    FOnAttributePicked OnPicked(FOnAttributePicked::CreateRaw(this, &SBattleEventPropertyWidget::OnAttributePicked));

    return SNew(SBox)
        .WidthOverride(280)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            .MaxHeight(500)
            [
                SNew(Pokemon::SAttributeListWidget)
                    .OnAttributePickedDelegate(OnPicked)
                    .StructType(StructType)
            ]
        ];
}

FText SBattleEventPropertyWidget::GetSelectedValueAsString() const {
    if (SelectedProperty) {
        auto Class = SelectedProperty->GetOwnerStruct();
        FString PropertyString = FString::Printf(TEXT("%s.%s"), *Class->GetName(), *SelectedProperty->GetName());
        return FText::FromString(PropertyString);
    }

    return FText::FromString(TEXT("None"));
}

void SBattleEventPropertyWidget::OnAttributePicked(FProperty *InProperty) {
    if (OnAttributeChanged.IsBound()) {
        OnAttributeChanged.Execute(InProperty);
    }

    // Update the selected item for displaying
    SelectedProperty = InProperty;

    // close the list
    ComboButton->SetIsOpen(false);
}