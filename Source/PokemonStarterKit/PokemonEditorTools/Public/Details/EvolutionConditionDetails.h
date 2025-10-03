// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * @brief A property type identifier used to determine if a property type is customized for EvolutionCondition
 * structures.
 *
 * This class identifies whether a given property type should be customized in the Unreal Engine property editor,
 * specifically for EvolutionCondition structures, leveraging the IPropertyTypeIdentifier interface.
 *
 * The identifier checks the type of the property to see if it matches a specific structure,
 * enabling conditional customization for certain properties.
 */
class POKEMONEDITORTOOLS_API FEvolutionConditionPropertyIdentifier final : public IPropertyTypeIdentifier
{
  public:
    bool IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const override;
};

/**
 * @brief Provides custom property type customization for EvolutionCondition structures in the Unreal Engine editor.
 *
 * This class extends the IPropertyTypeCustomization interface to define how properties of EvolutionCondition
 * structures are displayed and interacted with in the property editor.
 *
 * The header and child properties of EvolutionCondition structures are customized using the CustomizeHeader
 * and CustomizeChildren methods. It ensures proper widget representation and functionality for properties,
 * including inline editing and handling property-specific callbacks.
 *
 * The OnMethodChanged static method provides a callback for handling changes in the "Method" property, enabling
 * dynamic adjustments or updates to associated properties based on user modifications.
 */
class POKEMONEDITORTOOLS_API FEvolutionConditionDetails final : public IPropertyTypeCustomization
{
  public:
    /**
     * @brief Creates an instance of the property type customization for EvolutionCondition structures.
     *
     * This method provides a shared reference to a new instance of FEvolutionConditionDetails, which defines the custom
     * behavior and display options for the EvolutionCondition structure in the Unreal Engine property editor.
     *
     * It supports the registration of a customized property type layout for EvolutionCondition-related properties.
     *
     * @return A shared reference to the newly created FEvolutionConditionDetails instance.
     */
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    void CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                         IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                           IPropertyTypeCustomizationUtils &CustomizationUtils) override;

  private:
    static void OnMethodChanged(TSharedRef<IPropertyHandle> ParentProperty, TSharedRef<IPropertyHandle> MethodProperty);
};