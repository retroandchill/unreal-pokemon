// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h" 	
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"

class UPaperZDAnimBP;
class UPaperZDAnimationSource;

enum class EPaperZDModeSection : uint8
{
	Blueprint,
	Animation
};

/**
 * Creates a simple asset selector widget that enables quick changes between an AnimationSource and its related Animation Blueprints.
 */
class SPaperZDModeSelectorWidget: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperZDModeSelectorWidget)
		: _DefaultAnimBP(nullptr)
		, _bSourceSelected(true)
	{}
	/* The animation blueprint to select by default if just the "blueprint" button is pressed and not an entry of the combo menu. */
	SLATE_ARGUMENT(UPaperZDAnimBP*, DefaultAnimBP)

	/* If true, the widget will display itself as highlighting the "Animation" section. */
	SLATE_ARGUMENT(bool, bSourceSelected)
	SLATE_END_ARGS()

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, UPaperZDAnimationSource* InAnimSource);
	~SPaperZDModeSelectorWidget();

private:
	/* Called when the checkbox is triggered. */
	void OnCheckboxStateChanged(ECheckBoxState InState, EPaperZDModeSection Section);

	/* Callback triggered when a default animation blueprint is trying to be setup. */
	void OnDefaultAnimationBlueprintSet(UPaperZDAnimBP* InDefaultBP);

	/* Called to show the dropdown menu from the blueprint bar. */
	TSharedRef<SWidget> HandleGetMenuContent();

	/* Called when we select an asset from the picker. */
	void HandleAssetSelectedFromPicker(const FAssetData& SelectedAsset);

private:	
	//Arguments
	TObjectPtr<UPaperZDAnimBP>				DefaultAnimBP;
	TObjectPtr<UPaperZDAnimationSource>		AnimationSource;
	bool									bSourceSelected;
};
