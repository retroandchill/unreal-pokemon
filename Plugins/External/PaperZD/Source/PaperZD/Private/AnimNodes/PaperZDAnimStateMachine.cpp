// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimNodes/PaperZDAnimStateMachine.h"
#include "PaperZDAnimBPGeneratedClass.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimStateMachine)
#endif

bool FPaperZDAnimStateMachineTransitionRule::EvaluateRule(UObject* AnimInstance) const
{
	if (bDynamicRule)
	{
		if (UFunction* Function = AnimInstance->FindFunction(RuleFunctionName))
		{
			//Create Buffer and call function
			uint8* Buffer = (uint8*)FMemory_Alloca(Function->ParmsSize);
			FMemory::Memzero(Buffer, Function->ParmsSize);
			AnimInstance->ProcessEvent(Function, Buffer);

			//Obtain the return value (Out Parameters)
			bool bRuleValue = false;
			for (TFieldIterator<FProperty> PropIt(Function, EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
			{
				FProperty* Property = *PropIt;
				if (Property->HasAnyPropertyFlags(CPF_OutParm)) {
					uint8* OutValueAddr = Property->ContainerPtrToValuePtr<uint8>(Buffer);
					bool* pReturn = (bool*)OutValueAddr;
					bRuleValue = *pReturn;
					break;
				}
			}

			return bRuleValue;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot find Transition Rule function named '%s' on '%s'"), *RuleFunctionName.ToString(), *AnimInstance->GetName());
			return false;
		}
	}
	else
	{
		return bConstantValue;
	}	
}