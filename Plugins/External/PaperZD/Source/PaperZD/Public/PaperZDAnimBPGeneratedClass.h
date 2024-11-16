// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "AnimNodes/PaperZDAnimStateMachine.h"
#include "PaperZDAnimBPGeneratedClass.generated.h"

struct FPaperZDAnimNode_Sink;
struct FPaperZDAnimNode_StateMachine;
class UPaperZDAnimationSource;

/**
 * Structure that holds the debug data for a given AnimBP class
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimBPDebugData
{
	GENERATED_BODY()
};

/* Holds the descriptor for a slot override that has been registered to the class. */
USTRUCT()
struct FPaperZDOverrideSlotDescriptor
{
	GENERATED_BODY()

	/* Name of the slot, unique. */
	UPROPERTY()
	FName SlotName;

	/* Name of the group that the slot belongs to. */
	UPROPERTY()
	FName GroupName;
};

/**
 * Generated class for the PaperZDAnimBP.
 */
UCLASS()
class PAPERZD_API UPaperZDAnimBPGeneratedClass : public UBlueprintGeneratedClass
{
	GENERATED_BODY()

	/* Grant access to the compile time accessor. */
	friend class FPaperZDAnimBPGeneratedClassAccess;
	friend class FPaperZDAnimBPCompilerContext;

	/* The animation source that the class has been compiled to support. */
	UPROPERTY()
	TObjectPtr<UPaperZDAnimationSource> SupportedAnimationSource;

	/* List of the exposed value handlers. */
	UPROPERTY()
	TArray<FPaperZDExposedValueHandler> EvaluateGraphExposedInputs;

	/* List of state machine definitions. */
	UPROPERTY()
	TArray<FPaperZDAnimStateMachine> StateMachines;

	/* Mapping between Custom AnimNotify name to function name. */
	UPROPERTY()
	TMap<FName, FName> AnimNotifyFunctionMapping;

	/* List of registered override animation slots for this class, keyed by the slot name */
	UPROPERTY()
	TMap<FName, FPaperZDOverrideSlotDescriptor> RegisteredOverrideSlots;

	/* Transient linkage data (generated during the LINK stage). */
	TArray<FStructProperty*> AnimNodeProperties;
	TArray<FStructProperty*> StateMachineNodeProperties;

	/* Pointer to the root node property. */
	FStructProperty* RootNodeProperty;

public:
	//ctor
	UPaperZDAnimBPGeneratedClass();

	// UStruct interface
	virtual void Link(FArchive& Ar, bool bRelinkExistingProperties) override;
	// End of UStruct interface

	// UClass interface
	virtual void PurgeClass(bool bRecompilingOnLoad) override;
	virtual void PostLoadDefaultObject(UObject* Object) override;
	// End of UClass interface

	/* Called after a link, to cache the nodes that require special treatment. */
	void CacheRequiredNodes(UObject* DefaultObject);

	/* Obtain the root node from an AnimInstance object. */
	FPaperZDAnimNode_Sink* GetRootNode(UObject* AnimInstanceObject) const;

	/* Obtain a list of the StateMachine nodes that live on the AnimInstance. */
	TArray<FPaperZDAnimNode_StateMachine*> GetStateMachineNodes(UObject* AnimInstanceObject) const;

	/* Finds the function implementation for the AnimNotify with the given name. */
	UFunction* FindAnimNotifyFunction(FName AnimNotifyName) const;

	/* Obtain the AnimNode that is linked by the given LinkID. */
	FPaperZDAnimNode_Base* GetAnimNodeByLinkID(UObject* AnimInstanceObject, int32 LinkID) const;

	/* Obtain the AnimNode by the given PropertyIndex obtained at compilation time, order is reversed against node discovery. */
	FPaperZDAnimNode_Base* GetAnimNodeByPropertyIndex(UObject* AnimInstanceObject, int32 Index) const;

	/* Obtains the AnimNode by the given PropertyIndex obtained at compilation time and casts it to the given template class. */
	template <typename T>
	T* GetAnimNodeByPropertyIndex(UObject* AnimInstanceObject, int32 Index) const
	{
		return static_cast<T*>(GetAnimNodeByPropertyIndex(AnimInstanceObject, Index));
	}
	
	/* Get the list of state machine definitions. */
	const TArray<FPaperZDAnimStateMachine>& GetStateMachines() const;

	/* Obtain the type of AnimSequence supported by this class. */
	const UPaperZDAnimationSource* GetSupportedAnimationSource() const;

	/* Check if the given slot has been registered. */
	bool GetOverrideSlotDescriptor(FName SlotName, FPaperZDOverrideSlotDescriptor& SlotDescriptor) const;
};

/* Helper function to quickly obtain the ZD AnimGeneratedClass from the given object. */
FORCEINLINE UPaperZDAnimBPGeneratedClass* GetAnimClass(UObject* AnimInstance)
{
	if (AnimInstance)
	{
		return Cast<UPaperZDAnimBPGeneratedClass>(AnimInstance->GetClass());
	}

	return nullptr;
}
