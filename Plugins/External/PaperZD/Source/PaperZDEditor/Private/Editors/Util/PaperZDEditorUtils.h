// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"

class UBlueprint;
class UEdGraph;

/**
 * Assorted utility class for the Editor module
 */
class FPaperZDEditorUtils
{
public:	
	/* Handles deletion of a Animation related Graph. */
	static void RemoveGraph(UBlueprint* Blueprint, class UEdGraph* GraphToRemove);
};
