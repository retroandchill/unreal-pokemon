// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class RPGEDITOR_API FRPGComponentInitializerCustomization : public IPropertyTypeCustomization
{
  public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    TSharedPtr<IPropertyHandle> OuterPropertyHandle;

    TSharedRef<SWidget> MakeFunctionPicker() const;

    FText GetCurrentFunctionText() const;

    static bool IsValidInitFunction(const UFunction *Func, const UScriptStruct *ExpectedStruct);
};
