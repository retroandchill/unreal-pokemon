// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "PaperZDNotifyGraph.generated.h"

/**
 * A graph that represents an implementation of an Animation Source "Custom notify", on the parent AnimBP.
 */
UCLASS()
class UPaperZDNotifyGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	/* The name of the notify we're implementing. */
	UPROPERTY()
	FName NotifyName;

	/* True if the notify graph belongs to a valid registered notify, otherwise it belongs to an unregistered notify that hasn't been removed yet. */
	UPROPERTY()
	bool bRegistered;
};
