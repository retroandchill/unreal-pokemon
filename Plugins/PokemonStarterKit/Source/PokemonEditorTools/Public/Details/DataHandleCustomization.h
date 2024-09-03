// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class POKEMONEDITORTOOLS_API FDataHandleCustomization : public IPropertyTypeCustomization {
  public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;
};
