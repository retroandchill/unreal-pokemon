// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Detail customizer for the FPokemonDTO class.
 */
class POKEMONCOREEDITOR_API FPokemonDTODetails : public IPropertyTypeCustomization {
public:
    /**
     * Create a new instance of the customizer
     * @return The created customizer instance
     */
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
    
    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;

};
