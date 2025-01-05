// "Unreal Pokémon" created by Retro & Chill.

#include "Pins/PocketKeyPin.h"
#include "PokemonDataSettings.h"
#include "RetroLib/Ranges/Algorithm/FindFirst.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Compatibility/Array.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "SSearchableComboBox.h"
#include "Strings/StringUtilities.h"

class UPokemonDataSettings;

void SPocketKeyPin::Construct(const FArguments &, UEdGraphPin *InGraphPin) {
    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPin);
}

void SPocketKeyPin::ParseDefaultValueData() {
    Handle.FromExportString(GraphPinObj->GetDefaultAsString(), PPF_SerializedAsImportText);
}

TSharedRef<SWidget> SPocketKeyPin::GetDefaultValueWidget() {
    ParseDefaultValueData();
    // clang-format off
    Options = GetDefault<UPokemonDataSettings>()->PocketNames |
              Retro::Ranges::Views::Values |
              Retro::Ranges::Views::Transform(&UStringUtilities::NameToStringPtr) |
              Retro::Ranges::To<TArray>();
    // clang-format on

    // clang-format off
    auto Match = Options |
                 Retro::Ranges::AnyOf(Retro::BindFront<&SPocketKeyPin::RowMatches>(this));
    // clang-format on
    if (!Match && !Options.IsEmpty()) {
        Handle.PocketName = **Options[0];
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
            Handle.PocketName = FName(*Name);
            auto NewValue = Handle.ExportText();
            const FScopedTransaction Transaction(NSLOCTEXT("GraphEditor", "ChangePinValue", "Change Pin Value"));
            GraphPinObj->Modify();
            GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, NewValue);
        })
        .InitiallySelectedItem(Item)
        .Content()
        [
            SNew(STextBlock)
            .Text(this, &SPocketKeyPin::GetItemText)
            .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ];
    // clang-format on
}

bool SPocketKeyPin::RowMatches(const TSharedPtr<FString> &Str) const {
    return FName(*Str) == Handle.PocketName;
}

const TSharedPtr<FString> &SPocketKeyPin::GetItemString() const {
    // clang-format off
    auto Item = Options |
                Retro::Ranges::Views::Filter(Retro::BindFront<&SPocketKeyPin::RowMatches>(this)) |
                Retro::Ranges::FindFirst();
    // clang-format on
    check(Item.IsSet())
    return *Item;
}

FText SPocketKeyPin::GetItemText() const {
    return FText::FromString(*GetItemString());
}