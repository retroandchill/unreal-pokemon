// "Unreal Pokémon" created by Retro & Chill.

#include "Pins/DataHandlePinStructPin.h"
#include "RetroLib/Ranges/Algorithm/FindFirst.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "SSearchableComboBox.h"

void SDataHandlePinStructPin::Construct(const FArguments &, UEdGraphPin *InGraphPin)
{
    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPin);
}

void SDataHandlePinStructPin::ParseDefaultValueData()
{
    Handle = CastChecked<UScriptStruct>(GraphPinObj->PinType.PinSubCategoryObject.Get());
    Handle.FromExportString(GraphPinObj->GetDefaultAsString(), PPF_SerializedAsImportText);
}

TSharedRef<SWidget> SDataHandlePinStructPin::GetDefaultValueWidget()
{
    ParseDefaultValueData();
    Options = Handle.GetStructOptions();

    if (!Retro::Ranges::AnyOf(Options, Retro::BindMethod<&SDataHandlePinStructPin::RowMatches>(this)) &&
        !Options.IsEmpty())
    {
        Handle.SetRowID(FName(**Options[0]));
    }

    auto &Item = GetItemString();

    // clang-format off
    return SNew(SSearchableComboBox)
        .Visibility(this, &SGraphPin::GetDefaultValueVisibility)
        .OptionsSource(&Options)
        .OnGenerateWidget_Lambda([](const TSharedPtr<FString> &Name) {
            return SNew(STextBlock)
                .Text(FText::FromString(*Name))
                .Font(IPropertyTypeCustomizationUtils::GetRegularFont());
        })
        .OnSelectionChanged_Lambda([this](const TSharedPtr<FString> &Name, ESelectInfo::Type) {
            Handle.SetRowID(FName(*Name));
            auto NewValue = Handle.ExportText();
            const FScopedTransaction Transaction(NSLOCTEXT("GraphEditor", "ChangePinValue", "Change Pin Value"));
            GraphPinObj->Modify();
            GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, NewValue);
        })
        .InitiallySelectedItem(Item)
        .Content()
        [
            SNew(STextBlock)
            .Text(this, &SDataHandlePinStructPin::GetItemText)
            .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ];
    // clang-format on
}

bool SDataHandlePinStructPin::RowMatches(const TSharedPtr<FString> &Str) const
{
    return FName(*Str) == Handle.GetRowID();
}

const TSharedPtr<FString> &SDataHandlePinStructPin::GetItemString() const
{
    // clang-format off
    auto Item = Options |
                Retro::Ranges::Views::Filter(Retro::BindMethod<&SDataHandlePinStructPin::RowMatches>(this)) |
                Retro::Ranges::FindFirst();
    // clang-format on
    check(Item.IsSet())
    return *Item;
}

FText SDataHandlePinStructPin::GetItemText() const
{
    return FText::FromString(*GetItemString());
}