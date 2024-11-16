// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "PaperZDComponentReference.generated.h"

/**
 *	Allows references to the actor's own components via a specialized detail customization that works with CDOs.
 */
USTRUCT(BlueprintType)
struct FPaperZDComponentReference : public FComponentReference
{
	GENERATED_BODY()
};

/**
 *	Allows references to the actor's own components via a specialized detail customization that works with CDOs.
 */
USTRUCT(BlueprintType)
struct FPaperZDSoftComponentReference : public FSoftComponentReference
{
	GENERATED_BODY()
};
