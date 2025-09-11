// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class POKEMONEDITORTOOLS_API FEvolutionConditionPropertyIdentifier final : public IPropertyTypeIdentifier
{
  public:
    bool IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const override;
};

/**
 *
 */
class POKEMONEDITORTOOLS_API FEvolutionConditionDetails final : public IPropertyTypeCustomization
{
  public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    static void OnMethodChanged(TSharedRef<IPropertyHandle> ParentProperty, TSharedRef<IPropertyHandle> MethodProperty);
};