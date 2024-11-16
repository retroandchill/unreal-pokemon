// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimBP.h"
#include "PaperZDCustomVersion.h"
#include "PaperZDCharacter.h"

#if WITH_EDITOR
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"
#include "IPaperZDEditorProxy.h"
#include "PaperZDAnimBPGeneratedClass.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphSchema.h"
#include "UObject/UObjectIterator.h"
#include "PaperZDCharacter.h"
#endif

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimBP)
#endif

#if WITH_EDITOR
const FString UPaperZDAnimBP::SequenceNameTemplate("NewAnimSequence");
#endif

UPaperZDAnimBP::UPaperZDAnimBP(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	bRecompileOnLoad = true;
}

void UPaperZDAnimBP::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	const int32 ZDVersion = GetLinkerCustomVersion(FPaperZDCustomVersion::GUID);
	if (ZDVersion < FPaperZDCustomVersion::AnimSequencesAdded)
	{
		//To update the AnimBP to use AnimSequences we need info that's editor only (namely GraphNodes) to be updated, we need to use the proxy for this kind of operations
		//as the runtime module has no dependency on the editor module (and cannot have one)
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimSequences(this);
	}
	
	if (ZDVersion < FPaperZDCustomVersion::AnimNodeOuterFix)
	{
		//We need to use the proxy for this too, AnimGraphs must be updated
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimNodeOuterFix(this);
	}

	if (ZDVersion < FPaperZDCustomVersion::AnimSequenceCategoryAdded)
	{
		//We use the proxy so the updates are all on the proxy (the AnimBP class ends up being leaner)
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimSequenceCategoryAdded(this);
	}

	if (ZDVersion < FPaperZDCustomVersion::AnimSequenceAsStandaloneAsset)
	{
		//Call the proxy for updating version
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimSequenceAsStandaloneAsset(this);
	}

	if (ZDVersion < FPaperZDCustomVersion::AnimBlueprintRework)
	{
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimBlueprintRework(this);
	}

	if (ZDVersion < FPaperZDCustomVersion::AnimationSourceAdded)
	{
		IPaperZDEditorProxy::Get()->UpdateVersionToAnimationSourceAdded(this);
	}
#endif
}

void UPaperZDAnimBP::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FPaperZDCustomVersion::GUID);

#if WITH_EDITOR
	//We need the animation source to be loaded before this blueprint compiles
	if (SupportedAnimationSource)
	{
		Ar.Preload(SupportedAnimationSource);
	}
#endif
}

#if WITH_EDITOR

void UPaperZDAnimBP::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		//Create the graph if it doesn't exist
		CreateGraph();
	}
}

UClass* UPaperZDAnimBP::GetBlueprintClass() const
{
	return UPaperZDAnimBPGeneratedClass::StaticClass();
}
#endif // WITH_EDITOR

#if WITH_EDITOR
UEdGraph* UPaperZDAnimBP::GetGraph()
{
	return AnimationGraph;
}

void UPaperZDAnimBP::CreateGraph()
{
	if (AnimationGraph == nullptr)
	{
		AnimationGraph = IPaperZDEditorProxy::Get()->CreateNewAnimationGraph(this);

		// Give the schema a chance to fill out any required nodes (like the entry node or results node)
		const UEdGraphSchema* Schema = AnimationGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*AnimationGraph);
		AnimationGraph->bAllowDeletion = false;
		
		//@TODO: Check how to maintain the graph in the "last edited documents"
		LastEditedDocuments.Add(AnimationGraph.Get());
	}
}

void UPaperZDAnimBP::OnPreCompile()
{
	//Make sure the animation graph is removed from the UbergraphPages
	//as we want to be certain that we have full control on the AnimGraph compilation procedure
	//The graph will be manually added into the consolidated graph when compiling
	UbergraphPages.Remove(AnimationGraph);

	//Always make sure we have the notify functions up-to-date
	IPaperZDEditorProxy::Get()->UpdateNotifyFunctions(this);
}

void UPaperZDAnimBP::OnPostCompile()
{
	//Make sure the animation graph is present as an Ubergraph Page
	//This is the only way we can make sure the AnimBP shows the Animation Graph
	//without having to resort to either creating a custom SMyBlueprint implementation
	//or having the AnimationGraph as some sort of "FunctionGraph"
	UbergraphPages.AddUnique(AnimationGraph);

	//@TODO: should change the notification
	//Search for Characters using this AnimBP and notify them
// 	for (TObjectIterator<APaperZDCharacter> Itr; Itr; ++Itr)
// 	{
// 		// Access the subclass instance with the * or -> operators.
// 		APaperZDCharacter* Character = *Itr;
// 		if (Character->AnimationBlueprint == this)
// 		{
// 			Character->OnAnimBPCompiled();
// 		}
// 	}
	
	//@TODO: probably this will not be needed anymore, if the notifies are now on the GeneratedClass instead. CHECK
	//Mark as dirty, because if the transition function names do change, on reload they will not have been saved, but the new function will have been recompiled, giving error and forcing a manual recompile
	//MarkPackageDirty();
}

bool UPaperZDAnimBP::IsVersionCompatibleForCompilation() const
{
	const int32 ZDVersion = GetLinkerCustomVersion(FPaperZDCustomVersion::GUID);
	const bool bPostLoadCalled = !HasAnyFlags(RF_NeedPostLoad);

	//The linker version only gets stored after serialization, so the AnimBP could have already been patched up if the "PostLoad" method has been called.
	return ZDVersion >= FPaperZDCustomVersion::AnimBlueprintRework || bPostLoadCalled;
}

void UPaperZDAnimBP::OnRenameCustomNotify(FName OldName, FName NewName)
{
	IPaperZDEditorProxy::Get()->RenameCustomNotifyGraph(this, OldName, NewName);
}

#endif
