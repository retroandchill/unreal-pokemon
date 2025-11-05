// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class UNREALINJECTOREDITOR_API FInjectionTargetCustomization : public IPropertyTypeCustomization {
  public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    const UClass *OnGetClass() const;
    void OnSetClass(const UClass *NewClass) const;

    /** Handle to the property being customized */
    TSharedPtr<IPropertyHandle> PropertyHandle;
    /** A cache of the currently resolved value for the class name */
    mutable TWeakObjectPtr<const UClass> CachedClassPtr;
};
