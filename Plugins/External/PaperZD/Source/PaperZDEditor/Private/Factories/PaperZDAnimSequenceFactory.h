// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "ClassViewerFilter.h"
#include "PaperZDAnimSequenceFactory.generated.h"

struct FAssetData;
class SWindow;

UCLASS()
class UPaperZDAnimSequenceFactory : public UFactory
{
	GENERATED_BODY()

	/* Picked window Ptr */
	TSharedPtr<SWindow> PickerWindow;
		
public:
	/**
	 * Animation source for which to create this sequence, the sequence class will be inferred by the sequence type supported by the animation source. 
	 * If no object is set, the factory will display an asset picker to let the user select the target animation blueprint.
	 */
	UPROPERTY()
	TObjectPtr<class UPaperZDAnimationSource> TargetAnimSource;

public:
	UPaperZDAnimSequenceFactory(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override { return true; }
	//~ Begin UFactory Interface

private:
	/* Called when the user selects an AnimationSource from the asset picker. */
	void OnTargetAnimSourceSelected(const FAssetData& SelectedAsset);
};
