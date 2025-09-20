// "Unreal Pokémon" created by Retro & Chill.

#include "Handles/DataHandlePinStructPin.h"
#include "Interop/GameDataTypeManagedCallbacks.h"
#include "Misc/OutputDeviceNull.h"
#include "RangeV3.h"
#include "SSearchableComboBox.h"
#include "TypeGenerator/CSScriptStruct.h"

void SDataHandlePinStructPin::Construct(const FArguments &, UEdGraphPin *InGraphPin)
{
    SGraphPin::Construct(SGraphPin::FArguments(), InGraphPin);
}

void SDataHandlePinStructPin::ParseDefaultValueData()
{
    const auto *StructType = CastChecked<UScriptStruct>(GraphPinObj->PinType.PinSubCategoryObject.Get());
    Handle.InitializeAs(StructType);
    IDProperty = CastFieldChecked<FNameProperty>(StructType->FindPropertyByName(TEXT("ID")));

    FOutputDeviceNull NullOut;
    StructType->ImportText(*GraphPinObj->GetDefaultAsString(), Handle.GetMutableMemory(), nullptr,
                           PPF_SerializedAsImportText, &NullOut, StructType->GetFName().ToString(), true);
}

TSharedRef<SWidget> SDataHandlePinStructPin::GetDefaultValueWidget()
{
    ParseDefaultValueData();
    const auto *CSStruct = CastChecked<UCSScriptStruct>(Handle.GetScriptStruct());
    const auto ManagedStructType = CSStruct->GetManagedTypeInfo<>()->GetManagedTypeHandle();

    for (auto BaseOptions = FGameDataTypeManagedCallbacks::Get().GetDataHandleOptions(*ManagedStructType);
         const auto &Entry : BaseOptions)
    {
        auto &DisplayNameString = Entry.DisplayName.ToString();

        Options.Emplace(DisplayNameString, Entry);
        ComboBoxOptions.Emplace(MakeShared<FString>(DisplayNameString));
    }

    if (!ranges::any_of(Options | ranges::views::transform([](auto &Pair) { return Pair.Value; }) |
                            ranges::views::transform(&FDataHandleEntry::ID),
                        std::bind_front(&SDataHandlePinStructPin::RowMatches, this)) &&
        !Options.IsEmpty())
    {
        IDProperty->SetValue_InContainer(Handle.GetMutableMemory(), Options.begin()->Value.ID);
    }

    auto &Item = GetItemString();

    // clang-format off
    return SNew(SSearchableComboBox)
        .Visibility(this, &SGraphPin::GetDefaultValueVisibility)
        .OptionsSource(&ComboBoxOptions)
        .OnGenerateWidget_Lambda([](const TSharedPtr<FString> &Name) {
            return SNew(STextBlock)
                .Text(FText::FromString(*Name))
                .Font(IPropertyTypeCustomizationUtils::GetRegularFont());
        })
        .OnSelectionChanged_Lambda([this](const TSharedPtr<FString> &Name, ESelectInfo::Type) {
            const auto ID = Options.FindChecked(*Name).ID;
            IDProperty->SetValue_InContainer(Handle.GetMutableMemory(), ID);
            FString NewValue;
            Handle.GetScriptStruct()->ExportText(NewValue, Handle.GetMutableMemory(), Handle.GetMutableMemory(), nullptr, PPF_None, nullptr);
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

bool SDataHandlePinStructPin::RowMatches(const FName ID) const
{
    return ID == IDProperty->GetPropertyValue_InContainer(Handle.GetMemory());
}

const TSharedPtr<FString> &SDataHandlePinStructPin::GetItemString() const
{
    const auto Result = Algo::FindByPredicate(ComboBoxOptions, [this](const TSharedPtr<FString> &Str) {
        auto &[ID, DisplayName] = Options.FindChecked(*Str);
        return RowMatches(ID);
    });
    check(Result != nullptr)
    return *Result;
}

FText SDataHandlePinStructPin::GetItemText() const
{
    return FText::FromString(*GetItemString());
}