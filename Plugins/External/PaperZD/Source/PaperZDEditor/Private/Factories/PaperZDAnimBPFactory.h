// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Factories/Factory.h"
#include "PaperZDAnimBP.h"
#include "PaperZDAnimBPFactory.generated.h"

class UPaperZDAnimationSource;
class SWindow;
struct FAssetData;

/**
 * Factory for PaperZD Animation Blueprints.
 */
UCLASS()
class UPaperZDAnimBPFactory : public UFactory
{
	GENERATED_BODY()	

	// The type of blueprint that will be created
	TEnumAsByte<EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	TSubclassOf<UObject> ParentClass;

	/* Picked window Ptr */
	TSharedPtr<SWindow> PickerWindow;

	/* Animation source in which to base the new blueprint. */
	TObjectPtr<class UPaperZDAnimationSource> SupportedAnimationSource;

public:
	UPaperZDAnimBPFactory(const FObjectInitializer& ObjectInitializer);

	//~ Begin UFactory Interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface

private:
	/* Called when the user selects an AnimationSource from the asset picker. */
	void OnTargetAnimSourceSelected(const FAssetData& SelectedAsset);
};
