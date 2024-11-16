// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimBPGeneratedClass.h"
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_Sink.h"
#include "AnimNodes/PaperZDAnimNode_StateMachine.h"
#include "UObject/Package.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimBPGeneratedClass)
#endif

UPaperZDAnimBPGeneratedClass::UPaperZDAnimBPGeneratedClass()
	: Super()
{}

void UPaperZDAnimBPGeneratedClass::Link(FArchive& Ar, bool bRelinkExistingProperties)
{
	Super::Link(Ar, bRelinkExistingProperties);

	//Link called twice, make sure arrays are emptied
	AnimNodeProperties.Empty();
	StateMachineNodeProperties.Empty();

#if WITH_EDITORONLY_DATA
	for (FPaperZDExposedValueHandler& Handler : EvaluateGraphExposedInputs)
	{
		// handle potential renames of the class package
		Handler.ValueHandlerNodeProperty.ResolveWithRenamedStructPackage(this);
		check(Handler.ValueHandlerNodeProperty.Get() || Handler.ValueHandlerNodeProperty.IsPathToFieldEmpty());
	}
#endif // WITH_EDITORONLY_DATA

	//Create the property fields, they will have the same order as they had on the compiler, so the LinkIDs will be the same
	for (TFieldIterator<FProperty> It(this); It; ++It)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*It))
		{
			if (StructProp->Struct->IsChildOf(FPaperZDAnimNode_Base::StaticStruct()))
			{
				AnimNodeProperties.Add(StructProp);
			}

			//Add special nodes as well
			if (StructProp->Struct->IsChildOf(FPaperZDAnimNode_StateMachine::StaticStruct()))
			{
				StateMachineNodeProperties.Add(StructProp);
			}
		}
	}
}

void UPaperZDAnimBPGeneratedClass::PurgeClass(bool bRecompilingOnLoad)
{
	Super::PurgeClass(bRecompilingOnLoad);

	//Clean data
	AnimNodeProperties.Empty();
	EvaluateGraphExposedInputs.Empty();
	StateMachines.Empty();
	AnimNotifyFunctionMapping.Empty();
	RegisteredOverrideSlots.Empty();
	RootNodeProperty = nullptr;
	SupportedAnimationSource = nullptr;
}

void UPaperZDAnimBPGeneratedClass::PostLoadDefaultObject(UObject* Object)
{
	Super::PostLoadDefaultObject(Object);

	//Initialize CDO
	UPaperZDAnimBPGeneratedClass* Iter = this;
	while (Iter)
	{
		FPaperZDExposedValueHandler::InitClass(Iter->EvaluateGraphExposedInputs, Object);
		Iter = Cast<UPaperZDAnimBPGeneratedClass>(Iter->GetSuperClass());
	}

	CacheRequiredNodes(Object);
}

void UPaperZDAnimBPGeneratedClass::CacheRequiredNodes(UObject* DefaultObject)
{
	for (FStructProperty* StructProp : AnimNodeProperties)
	{
		if (StructProp->Struct == FPaperZDAnimNode_Sink::StaticStruct())
		{
			//We're dealing with a sink node, check if this is actually the main sink
			FPaperZDAnimNode_Sink* SinkNode = StructProp->ContainerPtrToValuePtr<FPaperZDAnimNode_Sink>(DefaultObject);
			if (SinkNode && SinkNode->IsMainSinkNode())
			{
				RootNodeProperty = StructProp;
			}
		}
	}
}

FPaperZDAnimNode_Sink* UPaperZDAnimBPGeneratedClass::GetRootNode(UObject* AnimInstanceObject) const
{
	FPaperZDAnimNode_Sink* RootNode = nullptr;
	if (RootNodeProperty)
	{
		RootNode = RootNodeProperty->ContainerPtrToValuePtr<FPaperZDAnimNode_Sink>(AnimInstanceObject);
	}

	return RootNode;
}

TArray<FPaperZDAnimNode_StateMachine*> UPaperZDAnimBPGeneratedClass::GetStateMachineNodes(UObject* AnimInstanceObject) const
{
	TArray<FPaperZDAnimNode_StateMachine*> StateMachineNodes;
	for (FStructProperty* StructProp : StateMachineNodeProperties)
	{
		StateMachineNodes.Add(StructProp->ContainerPtrToValuePtr<FPaperZDAnimNode_StateMachine>(AnimInstanceObject));
	}

	return StateMachineNodes;
}

UFunction* UPaperZDAnimBPGeneratedClass::FindAnimNotifyFunction(FName AnimNotifyName) const
{
	const FName* pAnimFunctionName = AnimNotifyFunctionMapping.Find(AnimNotifyName);
	if (pAnimFunctionName)
	{
		return FindFunctionByName(*pAnimFunctionName);
	}

	return nullptr;
}

FPaperZDAnimNode_Base* UPaperZDAnimBPGeneratedClass::GetAnimNodeByLinkID(UObject* AnimInstanceObject, int32 LinkID) const
{
	FPaperZDAnimNode_Base* AnimNode = nullptr;
	if (AnimNodeProperties.IsValidIndex(LinkID))
	{
		FStructProperty* LinkedProperty = AnimNodeProperties[LinkID];
		AnimNode = LinkedProperty->ContainerPtrToValuePtr<FPaperZDAnimNode_Base>(AnimInstanceObject);
	}

	return AnimNode;
}

FPaperZDAnimNode_Base* UPaperZDAnimBPGeneratedClass::GetAnimNodeByPropertyIndex(UObject* AnimInstanceObject, int32 Index) const
{
	FPaperZDAnimNode_Base* AnimNode = nullptr;
	if (AnimNodeProperties.IsValidIndex(Index))
	{
		FStructProperty* LinkedProperty = AnimNodeProperties.Last(Index);
		AnimNode = LinkedProperty->ContainerPtrToValuePtr<FPaperZDAnimNode_Base>(AnimInstanceObject);
	}

	return AnimNode;
}

const TArray<FPaperZDAnimStateMachine>& UPaperZDAnimBPGeneratedClass::GetStateMachines() const
{
	return StateMachines;
}

const UPaperZDAnimationSource* UPaperZDAnimBPGeneratedClass::GetSupportedAnimationSource() const
{
	return SupportedAnimationSource;
}

bool UPaperZDAnimBPGeneratedClass::GetOverrideSlotDescriptor(FName SlotName, FPaperZDOverrideSlotDescriptor& SlotDescriptor) const
{
	const FPaperZDOverrideSlotDescriptor* pDescriptor = RegisteredOverrideSlots.Find(SlotName);
	if (pDescriptor)
	{
		SlotDescriptor = *pDescriptor;
	}

	return pDescriptor != nullptr;
}
