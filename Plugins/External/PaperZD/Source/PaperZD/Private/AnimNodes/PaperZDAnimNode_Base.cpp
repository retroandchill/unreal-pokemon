// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimBPGeneratedClass.h"
#include "PaperZDAnimInstance.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNode_Base)
#endif

//////////////////////////////////////////////////////////////////////////
// Animation Context
//////////////////////////////////////////////////////////////////////////
UPaperZDAnimBPGeneratedClass* FPaperZDAnimationBaseContext::GetAnimBPClass() const
{
	return AnimInstance ? CastChecked<UPaperZDAnimBPGeneratedClass>(AnimInstance->GetClass()) : nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Animation Data Link
//////////////////////////////////////////////////////////////////////////
void FPaperZDAnimDataLink::Initialize(const FPaperZDAnimationInitContext& Context)
{
	//Attempt to re link the node
	if (AnimNode == nullptr && LinkID != INDEX_NONE)
	{
		UPaperZDAnimBPGeneratedClass* AnimClass = CastChecked<UPaperZDAnimBPGeneratedClass>(Context.AnimInstance->GetClass());
		AnimNode = AnimClass->GetAnimNodeByLinkID(Context.AnimInstance, LinkID);
	}

	//If we have a node, we should initialize it
	if (AnimNode)
	{
		AnimNode->Initialize(Context);
	}
}

void FPaperZDAnimDataLink::Update(const FPaperZDAnimationUpdateContext& Context)
{
	//@NOTE: Possible DebugData stuff here

	if (AnimNode)
	{
		AnimNode->Update(Context);
	}
}

void FPaperZDAnimDataLink::Evaluate(FPaperZDAnimationPlaybackData& OutputData)
{
	if (AnimNode)
	{
		AnimNode->Evaluate(OutputData);
	}
}

//////////////////////////////////////////////////////////////////////////
// Exposed value handler
//////////////////////////////////////////////////////////////////////////
void FPaperZDExposedValueHandler::Initialize(UClass* InClass)
{
	if (!bInitialized)
	{
		//Find the UFunction bound to this handler
		if (BoundFunction != NAME_None)
		{
			// This cached function is NULL when the CDO is initially serialized, or (in editor) when the class has been
			// recompiled and any instances have been re-instanced. When new instances are spawned, this function is
			// duplicated (it is a FProperty) onto those instances so we dont pay the cost of the FindFunction() call
#if !WITH_EDITOR
			if (Function == nullptr)
#endif
			{
				// we cant call FindFunction on anything but the game thread as it accesses a shared map in the object's class
				check(IsInGameThread());
				Function = InClass->FindFunctionByName(BoundFunction);
				check(Function);
			}
		}

		//Initialization complete
		bInitialized = true;
	}
}

void FPaperZDExposedValueHandler::Update(FPaperZDAnimationBaseContext& Context)
{
	if (Function)
	{
		Context.AnimInstance->ProcessEvent(Function, nullptr);
	}
}

void FPaperZDExposedValueHandler::InitClass(TArray<FPaperZDExposedValueHandler>& ValueHandlers, UObject* InClassDefaultObject)
{
	for (FPaperZDExposedValueHandler& Handler : ValueHandlers)
	{
		FPaperZDAnimNode_Base* AnimNode = Handler.ValueHandlerNodeProperty->ContainerPtrToValuePtr<FPaperZDAnimNode_Base>(InClassDefaultObject);
		AnimNode->ExposedValueHandler = &Handler;
		Handler.Initialize(InClassDefaultObject->GetClass());
	}
}

//////////////////////////////////////////////////////////////////////////
// AnimNode
//////////////////////////////////////////////////////////////////////////
FPaperZDAnimNode_Base::FPaperZDAnimNode_Base()
	: ExposedValueHandler(nullptr)
{}

void FPaperZDAnimNode_Base::Initialize(const FPaperZDAnimationInitContext& Context)
{
	//Run evaluation handlers first, if they exist
	if (ExposedValueHandler != nullptr)
	{
		FPaperZDAnimationBaseContext BaseContext = Context;
		ExposedValueHandler->Update(BaseContext);
	}

	OnInitialize(Context);
}

void FPaperZDAnimNode_Base::Update(const FPaperZDAnimationUpdateContext& Context)
{
	//Run evaluation handlers first, if they exist
	if (ExposedValueHandler != nullptr)
	{
		FPaperZDAnimationBaseContext BaseContext = Context;
		ExposedValueHandler->Update(BaseContext);
	}

	//Pass down to child classes
	OnUpdate(Context);
}

void FPaperZDAnimNode_Base::Evaluate(FPaperZDAnimationPlaybackData& OutAnimationData)
{
	OnEvaluate(OutAnimationData);
}
