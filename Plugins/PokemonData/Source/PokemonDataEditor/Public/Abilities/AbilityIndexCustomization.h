// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Utility/AbilityIndex.h"

/**
 * 
 */
class POKEMONDATAEDITOR_API FAbilityIndexCustomization : public IPropertyTypeCustomization {
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
    
    void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;
    void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder &ChildBuilder,
        IPropertyTypeCustomizationUtils &CustomizationUtils) override;

private:
    /**
     * Generates a row of the combo widget
     *
     * @param	Option	Item string to be displayed in the combo item
     * @return	An SWidget representing the combo row.
     */
    TSharedRef<SWidget> OnGenerateComboWidget(TSharedPtr<FIntTextPair> Option);

    /**
     * Called when an item is selected in the combo box
     *
     * @param	InSelectedItem	String of the item selected
     * @param	SelectInfo		Type of selection
     * @param	HandleWeakPtr	Handle to the type property which will be changed by the combo selection
     */
    void OnComboSelectionChanged(TSharedPtr<FIntTextPair> InSelectedItem, ESelectInfo::Type SelectInfo, TWeakPtr<IPropertyHandle> HandleWeakPtr);
    
    TArray<TSharedPtr<FText>> Options;

};
