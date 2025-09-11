// Fill out your copyright notice in the Description page of Project Settings.

#include "Handles/DataHandleCustomization.h"
#include "DataHandle.h"
#include "DetailWidgetRow.h"
#include "Interop/GameDataTypeManagedCallbacks.h"
#include "SSearchableComboBox.h"
#include "TypeGenerator/CSScriptStruct.h"

TSharedRef<IPropertyTypeCustomization> FDataHandleCustomization::MakeInstance()
{
    return MakeShared<FDataHandleCustomization>();
}

void FDataHandleCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle,
                                               FDetailWidgetRow &HeaderRow,
                                               IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    Handle = PropertyHandle;

    // The C# class wraps an FName so in this case
    WrappedProperty = PropertyHandle->GetChildHandle("ID");
    TArray<void *> RawData;
    WrappedProperty->AccessRawData(RawData);
    TSet<FName> SelectedValues;
    for (const auto &RawValue : RawData)
    {
        SelectedValues.Add(*static_cast<FName *>(RawValue));
    }

    const auto StructProperty = CastFieldChecked<FStructProperty>(PropertyHandle->GetProperty());

    const auto *CSStruct = CastChecked<UCSScriptStruct>(StructProperty->Struct);
    const auto ManagedStructType = CSStruct->GetManagedTypeInfo<>()->GetManagedTypeHandle();

    if (AllowsNone())
    {
        ComboBoxOptions.Emplace(MakeShared<FString>(TEXT("")));
    }
    for (auto BaseOptions = FGameDataTypeManagedCallbacks::Get().GetDataHandleOptions(*ManagedStructType);
         const auto &Entry : BaseOptions)
    {
        auto &DisplayNameString = Entry.DisplayName.ToString();

        Options.Emplace(DisplayNameString, Entry);
        ComboBoxOptions.Emplace(MakeShared<FString>(DisplayNameString));
    }

    int32 DefaultIndex = INDEX_NONE;
    if (SelectedValues.Num() == 1)
    {
        const auto CurrentValue = *SelectedValues.begin();
        for (int32 i = 0; i < ComboBoxOptions.Num(); i++)
        {
            const auto &Option = *ComboBoxOptions[i];
            if (const auto *ExistingOption = Options.Find(Option);
                ExistingOption != nullptr && ExistingOption->ID != CurrentValue)
            {
                continue;
            }

            DefaultIndex = i;
            break;
        }
    }

    CurrentSelection = DefaultIndex != INDEX_NONE ? ComboBoxOptions[DefaultIndex] : nullptr;

    // clang-format off
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SSearchableComboBox)
                .OptionsSource(&ComboBoxOptions)
                .IsEnabled(this, &FDataHandleCustomization::IsEditEnabled)
                .OnGenerateWidget_Static(&FDataHandleCustomization::GenerateComboBoxEntry)
                .OnSelectionChanged(this, &FDataHandleCustomization::OnComboBoxSelectionChanged)
                .ContentPadding(3.0f)
                .InitiallySelectedItem(CurrentSelection)
                .Content()
                [
                    SNew(STextBlock)
                        .Text(this, &FDataHandleCustomization::GetComboBoxEntryText)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
    // clang-format on
}

void FDataHandleCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                 IDetailChildrenBuilder &ChildBuilder,
                                                 IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    // No children customization
}

bool FDataHandleCustomization::IsEditEnabled() const
{
    return Handle->IsValidHandle() && Handle->IsEditable();
}

TSharedRef<SWidget> FDataHandleCustomization::GenerateComboBoxEntry(TSharedPtr<FString> Value)
{

    return SNew(STextBlock)
        .Text(GetComboBoxEntryText(Value))
        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
}

void FDataHandleCustomization::OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type)
{
    CurrentSelection = NewSelection;
    TArray<void *> OuterStructs;
    Handle->AccessRawData(OuterStructs);

    Handle->NotifyPreChange();
    for (const auto Struct : OuterStructs)
    {
        const auto *Entry = Options.Find(*NewSelection);
        auto Name = Entry != nullptr ? Entry->ID : NAME_None;
        WrappedProperty->GetProperty()->SetValue_InContainer(Struct, &Name);
    }
    Handle->NotifyPostChange(EPropertyChangeType::ValueSet);
}

FText FDataHandleCustomization::GetComboBoxEntryText() const
{
    return CurrentSelection != nullptr ? GetComboBoxEntryText(CurrentSelection) : FText::GetEmpty();
}

FText FDataHandleCustomization::GetComboBoxEntryText(const TSharedPtr<FString> &Value)
{
    return Value != nullptr && !Value->IsEmpty() ? FText::FromString(*Value)
                                                 : NSLOCTEXT("FDataHandleCustomization", "None", "<None>");
}

bool FDataHandleCustomization::AllowsNone() const
{
    if (const auto *MapProperty = CastField<FMapProperty>(Handle->GetMetaDataProperty()))
    {
        if (const auto *ThisProperty = Handle->GetProperty(); ThisProperty == MapProperty->KeyProp)
        {
            // Do not allow none to be selected for map keys
            return false;
        }
    }

    if (Handle->HasMetaData(TEXT("AllowNone")))
    {
        return true;
    }

    return false;
}
