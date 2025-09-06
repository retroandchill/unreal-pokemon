// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Customization info about a wrapper class used for a key value.
 */
class POKEMONEDITORTOOLS_API FWrappedKeyCustomization : public IPropertyTypeCustomization
{
  public:
    /**
     * Create a new instance of this class with the wrapped property
     * @param Property The wrapped property in question
     */
    explicit FWrappedKeyCustomization(FName Property);

    static TSharedRef<IPropertyTypeCustomization> MakeInstance(FName PropertyName);

    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    /**
     * The name of the property used by the wrapped struct
     */
    FName PropertyName;
};
