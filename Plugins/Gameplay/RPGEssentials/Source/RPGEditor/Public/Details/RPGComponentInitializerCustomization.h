// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * @class FRPGComponentInitializerCustomization
 * @brief Provides customization functionality for initializing RPG components in the editor.
 *
 * This class is responsible for customizing the properties and behavior associated with RPG components during their
 * initialization process when working within the editor environment. It allows developers to create tailored tools,
 * adjust default values, and define how certain properties should be presented or configured.
 *
 * The customization logic facilitates control over how components are initialized and interacted with, enabling a more
 * dynamic and user-friendly editor experience.
 *
 * Responsibilities:
 * - Managing property customization for RPG components within the editor.
 * - Modifying default settings or adjusting specific initialization behaviors.
 * - Enhancing user interaction with component properties during initialization.
 *
 */
class RPGEDITOR_API FRPGComponentInitializerCustomization : public IPropertyTypeCustomization
{
  public:
    /**
     * @brief Creates and returns an instance of the FRPGComponentInitializerCustomization class.
     *
     * This static method is used to instantiate the customization class for RPG components.
     * It is typically registered with the property editor to enable custom property type handling
     * and interaction in the editor environment. The instance created is utilized to manage
     * how RPG component initializer properties are displayed and configured within the editor.
     *
     * @return A shared reference to an instance of FRPGComponentInitializerCustomization.
     */
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
