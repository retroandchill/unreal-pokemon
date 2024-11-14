// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "PaperZDAnimBPGeneratedClass.h"

/**
 * Accessor for modifying internal members of an Animation Blueprint "Generated Class" during compilation.
 */
class FPaperZDAnimBPGeneratedClassAccess
{
	/* Only grant access during compilation (private constructor, only the compiler can generate this accessor). */
	friend class FPaperZDAnimBPCompilerContext;
	FPaperZDAnimBPGeneratedClassAccess(UPaperZDAnimBPGeneratedClass* InGeneratedClass)
		: GeneratedClass(InGeneratedClass)
	{
		check(GeneratedClass);
	}

	/* Pointer to the generated class we're accessing. */
	TObjectPtr<UPaperZDAnimBPGeneratedClass> GeneratedClass;

public:
	/* Obtain the array of exposed value handlers (functions/properties). */
	TArray<FPaperZDExposedValueHandler>& GetExposedValueHandlers() const;

	/* Obtain the array of AnimStateMachine definitions. */
	TArray<FPaperZDAnimStateMachine>& GetStateMachines() const;

	/* Obtain the list of registered override slots, keyed by the slot name. */
	TMap<FName, FPaperZDOverrideSlotDescriptor>& GetRegisteredOverrideSlots() const;
};
