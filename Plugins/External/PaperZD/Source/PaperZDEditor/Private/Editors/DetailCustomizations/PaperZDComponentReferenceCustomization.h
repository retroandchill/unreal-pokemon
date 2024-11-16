// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "IPropertyTypeCustomization.h"
#include "Internationalization/Text.h"
#include "PropertyEditorModule.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class IPropertyHandle;
class SComboButton;
class SWidget;
class UActorComponent;
class UClass;
class UObject;
struct FComponentReference;
struct FSoftComponentReference;
struct FSlateBrush;

/**
 * Customization for the component reference that can be used on the blueprint editor with CDOs.
 * Modified from 'ComponentReferenceCustomization.h'
 */
class FPaperZDComponentReferenceCustomization : public IPropertyTypeCustomization
{
	/** The property handle we are customizing */
	TSharedPtr<IPropertyHandle> PropertyHandle;

	/** Main combo button */
	TSharedPtr<SComboButton> ComponentComboButton;
 
	/** Classes that can be used with this property */
	TArray<TSubclassOf<UActorComponent>> AllowedComponentClassFilters;

	/** Classes that can NOT be used with this property */
	TArray<TSubclassOf<UActorComponent>> DisallowedComponentClassFilters;

	/** Whether the asset can be 'None' in this case */
	bool bAllowClear;

	/** Do it has the UseComponentPicker metadata */
	bool bUseComponentPicker;

	/** Whether or not the component reference is a FSoftComponentReference */
	bool bIsSoftReference;

	/** Cached ComponentReference */
	TWeakObjectPtr<AActor> CachedFirstOuterActor;
	TWeakObjectPtr<const UActorComponent> CachedComponent;
	FPropertyAccess::Result CachedPropertyAccess;
	FName CachedVariableName;

public:
	/** Makes a new instance of this customization for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& PropertyTypeCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& PropertyTypeCustomizationUtils) override;

private:
	/** From the property metadata, build the list of allowed and disallowed class. */
	void BuildClassFilters();

	/** Build the combobox widget. */
	void BuildComboBox();

	/**
	 * From the Detail panel outer hierarchy, find the first actor or component owner we find.
	 * This is so we can reference the actor and obtain the compatible components on the owner.
	 */
	AActor* GetFirstOuterActor() const;

	/**
	 * Set the value of the asset referenced by this property editor.
	 * Will set the underlying property handle if there is one.
	 */
	void SetValue(const FComponentReference& Value);

	/** Get the value referenced by this widget. */
	FPropertyAccess::Result GetValue(FComponentReference& OutValue) const;

	/** Is the Value valid */
	bool IsComponentReferenceValid(const FComponentReference& Value) const;

	/** Callback when the property value changed. */
	void OnPropertyValueChanged();

	/**
	 * Return 0, if we have multiple values to edit.
	 * Return 1, if we display the widget normally.
	 */
	int32 OnGetComboContentWidgetIndex() const;

	bool CanEdit() const;
	bool CanEditChildren() const;

	const FSlateBrush* GetComponentIcon() const;
	FText OnGetComponentName() const;
	const FSlateBrush* GetStatusIcon() const;

	/**
	 * Get the content to be displayed in the asset/actor picker menu
	 * @returns the widget for the menu content
	 */
	TSharedRef<SWidget> OnGetMenuContent();

	/**
	 * Called when the asset menu is closed, we handle this to force the destruction of the asset menu to
	 * ensure any settings the user set are saved.
	 */
	void OnMenuOpenChanged(bool bOpen);

 	/**
 	 * Returns whether the component should be filtered out from selection.
 	 */
	bool IsFilteredComponent(const UActorComponent* const Component) const;
	static bool IsFilteredObject(const UObject* const Object, const TArray<TSubclassOf<UActorComponent>>& AllowedFilters, const TArray<TSubclassOf<UActorComponent>>& DisallowedFilters);

	/**
	 * Delegate for handling selection in the component picker.
	 * @param	SelectedComponent	The chosen component
	 * @param	VariableName		Name of the variable that holds the component, useful for CDOs as not all their variables are accessible easily.
	 */
	void OnComponentSelected(const UActorComponent* SelectedComponent, const FName VariableName);

	/**
	 * Handles when the component picker clears the previous selection.
	 */
	void OnClearSelection();

	/**
	 * Closes the combo button.
	 */
	void CloseComboButton();

	/**
	 * Obtains the component value, while dealing with Blueprint added components that might not be resolvable by the ComponentReference.
	 */
	 const UActorComponent* GetComponentFromReference(const FComponentReference& CompRef) const;

	 /**
	  * Obtains the cached variable name for the given component ref. Uses the currently cached component for reference.
	  * Intentionally non-const so it can't be used on high frequency getters (like slate's).
	  */
	  FName GetCachedComponentDisplayName(const FComponentReference& CompRef);
};
