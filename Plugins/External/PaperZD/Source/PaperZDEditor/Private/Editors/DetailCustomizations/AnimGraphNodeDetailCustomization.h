// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailPropertyExtensionHandler.h"
#include "IDetailCustomization.h"

class UPaperZDAnimGraphNode_Base;

/**
 * Shows an optional "ShowAsPin" inlined widget
 */
class FPaperZDAnimGraphNodeBindingExtension : public IDetailPropertyExtensionHandler
{
public:
	//~Begin IDetailPropertyExtensionHandler interface
	virtual bool IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const override;
	virtual TSharedRef<SWidget> GenerateExtensionWidget(const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> PropertyHandle) override;
	//~End IDetailPropertyExtensionHandler interface

private:
	// Helper function
	void GetOptionalPinData(const IPropertyHandle& PropertyHandle, int32& OutOptionalPinIndex, UPaperZDAnimGraphNode_Base*& OutAnimGraphNode) const;
};

/**
 * Detail customization for all the AnimGraph nodes
 */
class FPaperZDAnimGraphNodeDetailCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	//~Begin IDetailCustomization Interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	//~End IDetailCustomization Interface

	// Creates a widget for the supplied property
	TSharedRef<SWidget> CreatePropertyWidget(FProperty* TargetProperty, TSharedRef<IPropertyHandle> TargetPropertyHandle, UClass* NodeClass);

	/* Returns the visibility of the property, depending on if its shown as pin or not. */
	EVisibility GetVisibilityOfProperty(TSharedRef<IPropertyHandle> Handle) const;
};
