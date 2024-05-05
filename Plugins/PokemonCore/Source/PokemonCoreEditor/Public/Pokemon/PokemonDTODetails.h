// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class POKEMONCOREEDITOR_API FPokemonDTODetails : public IPropertyTypeCustomization {
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
    
    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;

private:
};
