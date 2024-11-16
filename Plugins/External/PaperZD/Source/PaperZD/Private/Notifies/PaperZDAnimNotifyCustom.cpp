// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDAnimNotifyCustom.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimBPGeneratedClass.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotifyCustom)
#endif

UPaperZDAnimNotifyCustom::UPaperZDAnimNotifyCustom(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDAnimNotifyCustom::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance /* = nullptr*/) const
{
	//Owning instance can be null on editor
	if (OwningInstance)
	{
		//Ask the instance for the function implementation
		UFunction* BoundFunction = OwningInstance->FindAnimNotifyFunction(Name);
		if (ensure(BoundFunction)) 
		{
			//Create Buffer and call function
			uint8 *Buffer = (uint8*)FMemory_Alloca(BoundFunction->ParmsSize);
			FMemory::Memzero(Buffer, BoundFunction->ParmsSize);
			OwningInstance->ProcessEvent(BoundFunction, Buffer);
		}
	}
}
