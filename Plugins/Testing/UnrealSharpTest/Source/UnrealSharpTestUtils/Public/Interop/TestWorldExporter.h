// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "TestWorld.h"
#include "UObject/Object.h"

#include "TestWorldExporter.generated.h"

/**
 * UTestWorldExporter is a utility class designed to manage lifecycle operations
 * related to FTestWorld instances, including initialization, finalization,
 * and beginning the play sequence.
 */
UCLASS()
class UNREALSHARPTESTUTILS_API UTestWorldExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * Initializes a test world instance, setting up the game instance and preparing the world
     * for operations. Optionally, it can immediately begin play mode for the world.
     *
     * @param World A reference to the FTestWorld instance to be initialized.
     * @param BeginPlay Specifies whether the world's gameplay sequence should start immediately upon initialization.
     */
    UNREALSHARP_FUNCTION()
    static void InitializeTestWorld(FTestWorld &World, bool BeginPlay);

    /**
     * Finalizes and cleans up resources associated with the given FTestWorld instance.
     *
     * @param World A reference to the FTestWorld instance to be finalized and cleaned up.
     */
    UNREALSHARP_FUNCTION()
    static void FinalizeTestWorld(FTestWorld &World);

    /**
     * Initiates the gameplay sequence for the given test world instance, preparing
     * it for game operations and setting its state to play mode.
     *
     * @param World A reference to the FTestWorld instance for which the gameplay sequence
     *              is to be initiated.
     */
    UNREALSHARP_FUNCTION()
    static void BeginPlay(FTestWorld &World);
};
