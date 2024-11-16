// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimationSourceFactory.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "AnimSequences/Sources/PaperZDAnimationSource_Flipbook.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ClassViewerFilter.h"
#include "Modules/ModuleManager.h"
#include "ClassViewerModule.h"
#include "Kismet2/SClassPickerDialog.h"
#include "UObject/UObjectIterator.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimationSourceFactory)
#endif

#define LOCTEXT_NAMESPACE "PaperZDAnimationSourceFactory"

//////////////////////////////////////////////////////////////////////////
//// Filter
//////////////////////////////////////////////////////////////////////////
class FAnimationSourceClassFilter : public IClassViewerFilter
{
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override
	{
		TSet<const UClass*> ChildOfSet;
		ChildOfSet.Add(UPaperZDAnimationSource::StaticClass());

		return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists) && InFilterFuncs->IfInChildOfClassesSet(ChildOfSet, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		TSet<const UClass*> ChildOfSet;
		ChildOfSet.Add(UPaperZDAnimationSource::StaticClass());

		return !InUnloadedClassData->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists) && InFilterFuncs->IfInChildOfClassesSet(ChildOfSet, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

//////////////////////////////////////////////////////////////////////////
//// Factory
//////////////////////////////////////////////////////////////////////////
UPaperZDAnimationSourceFactory::UPaperZDAnimationSourceFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = UPaperZDAnimationSource::StaticClass();
	SelectedClass = nullptr;
}

bool UPaperZDAnimationSourceFactory::ConfigureProperties()
{
	//First of all, we need an animation sequence class to be setup on the animation blueprint.
	UClass* PickedClass = nullptr;
	if (SelectedClass == nullptr && PickAnimationSourceClass(PickedClass))
	{
		SelectedClass = PickedClass;
		return true;
	}

	//either failed to pick a class or the user aborted.
	return false;
}

UObject* UPaperZDAnimationSourceFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UPaperZDAnimationSource* AnimationSourceNew = NewObject<UPaperZDAnimationSource>(InParent, SelectedClass, Name, Flags);
	return AnimationSourceNew;
}

bool UPaperZDAnimationSourceFactory::PickAnimationSourceClass(UClass*& PickedClass)
{
	if (ShouldDisplayPickerWindow())
	{
		//Create a picker for the class that will inherit from
		FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
		FClassViewerInitializationOptions InitOptions;
		InitOptions.Mode = EClassViewerMode::ClassPicker;

		//Initialize filter
		TSharedRef<FAnimationSourceClassFilter> Filter = MakeShareable<FAnimationSourceClassFilter>(new FAnimationSourceClassFilter);
		InitOptions.ClassFilters.Add(Filter);

		//View config
		const FText TitleText = LOCTEXT("SelectAnimationSourceClass", "Select the type of animation source to create.");
		return SClassPickerDialog::PickClass(TitleText, InitOptions, PickedClass, UPaperZDAnimationSource::StaticClass());
	}
	else
	{
		PickedClass = UPaperZDAnimationSource_Flipbook::StaticClass();
		return true;
	}
}

bool UPaperZDAnimationSourceFactory::ShouldDisplayPickerWindow() const
{
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* Class = *ClassIt;
		if (Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			continue;
		}

		// Check this class is a subclass of Base
		if (Class->IsChildOf(UPaperZDAnimationSource::StaticClass()) && Class != UPaperZDAnimationSource_Flipbook::StaticClass())
		{
			return true;
		}
	}

	return false;
}

#undef LOCTEXT_NAMESPACE