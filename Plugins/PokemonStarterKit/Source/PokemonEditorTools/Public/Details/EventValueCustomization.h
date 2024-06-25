// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class POKEMONEDITORTOOLS_API FEventValueCustomization : public IPropertyTypeCustomization  {
public:
    FEventValueCustomization(UScriptStruct* InStructType);
    
    static TSharedRef<IPropertyTypeCustomization> MakeInstance(UScriptStruct* StructType);

    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

private:
    TObjectPtr<UScriptStruct> StructType;
    
    // the attribute property
    TSharedPtr<IPropertyHandle> MyProperty;
    // the owner property
    TSharedPtr<IPropertyHandle> OwnerProperty;
    // the name property
    TSharedPtr<IPropertyHandle> NameProperty;

    TArray<TSharedPtr<FString>> PropertyOptions;

    TSharedPtr<FString>	GetPropertyType() const;

    void OnChangeProperty(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo);
    void OnAttributeChanged(FProperty* SelectedAttribute);
};
