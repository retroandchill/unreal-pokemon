// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataHandle.h"

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API FDataHandleCustomization final : public IPropertyTypeCustomization
{
  public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    bool IsEditEnabled() const;
    static TSharedRef<SWidget> GenerateComboBoxEntry(TSharedPtr<FString> Value);
    void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
    FText GetComboBoxEntryText() const;
    static FText GetComboBoxEntryText(const TSharedPtr<FString> & Value);
    bool AllowsNone() const;

    TSharedPtr<IPropertyHandle> Handle;
    TSharedPtr<IPropertyHandle> WrappedProperty;
    TMap<FString, FDataHandleEntry> Options;
    TArray<TSharedPtr<FString>> ComboBoxOptions;
    TSharedPtr<FString> CurrentSelection;
};
