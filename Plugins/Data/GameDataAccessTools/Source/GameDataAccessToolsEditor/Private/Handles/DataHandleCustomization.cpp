// Fill out your copyright notice in the Description page of Project Settings.

#include "Handles/DataHandleCustomization.h"
#include "DetailWidgetRow.h"
#include "SSearchableComboBox.h"

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
    const auto OptionsSource = StructProperty->Struct->GetMetaData("OptionsSource");
    const auto TargetFunction = FindObject<UFunction>(nullptr, *OptionsSource);
    const auto CDO = TargetFunction->GetOuterUClass()->GetDefaultObject();
    TArray<FName> Options;
    FFrame NewStack(CDO, TargetFunction, &Options, nullptr, TargetFunction->ChildProperties);
    TargetFunction->Invoke(CDO, NewStack, &Options);

    Algo::Transform(Options, ComboBoxOptions,
                    [](const FName &InName) { return MakeShared<FString>(InName.ToString()); });

    const int32 DefaultIndex = SelectedValues.Num() == 1 ? Options.Find(*SelectedValues.begin()) : INDEX_NONE;
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
    return Handle->IsValidHandle() && Handle->GetProperty()->HasAnyPropertyFlags(CPF_Edit);
}

TSharedRef<SWidget> FDataHandleCustomization::GenerateComboBoxEntry(TSharedPtr<FString> Value)
{
    return SNew(STextBlock).Text(FText::FromString(*Value)).Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
}

void FDataHandleCustomization::OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type)
{
    CurrentSelection = NewSelection;
    TArray<void *> OuterStructs;
    Handle->AccessRawData(OuterStructs);

    WrappedProperty->NotifyPreChange();
    for (const auto Struct : OuterStructs)
    {
        auto Name = FName(*NewSelection);
        WrappedProperty->GetProperty()->SetValue_InContainer(Struct, &Name);
    }
    WrappedProperty->NotifyPostChange(EPropertyChangeType::ValueSet);
}

FText FDataHandleCustomization::GetComboBoxEntryText() const
{
    return CurrentSelection != nullptr ? FText::FromString(*CurrentSelection) : FText::GetEmpty();
}
