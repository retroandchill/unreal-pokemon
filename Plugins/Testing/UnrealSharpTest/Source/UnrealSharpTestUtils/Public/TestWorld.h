// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SGameLayerManager.h"

/**
 * "Deleter" to shutdown the game instance so it doesn't cause issues when it gets garbage collected.
 */
struct FGameInstanceShutdown
{
    void operator()(UGameInstance *GameInstance) const
    {
        if (GameInstance != nullptr)
        {
            GameInstance->Shutdown();
        }
    }
};

/**
 * An RAII container for a Game Instance that calls shutdown when the object leaves scope.
 */
using FGameInstancePtr = TUniquePtr<UGameInstance, FGameInstanceShutdown>;

/**
 * "Deleter" used to clean up the world
 */
struct FCleanupWorld
{
    void operator()(UWorld *World) const
    {
        if (World != nullptr)
        {
            World->DestroyWorld(false);
        }
    }
};

/**
 * Smart pointer to the world that calls cleanup when it gets destroyed.
 */
using FWorldPtr = TUniquePtr<UWorld, FCleanupWorld>;

/**
 * A test class designed to simulate and manipulate a game world environment during testing scenarios.
 * Primarily used for validating game world interactions, behaviors, and logic under controlled conditions.
 */
class UNREALSHARPTESTUTILS_API FTestWorld
{
  public:
    /**
     * Constructs the FTestWorld object, initializing the game instance and world,
     * and optionally starting the world's gameplay sequence.
     *
     * @param bBeginPlay Specifies whether to immediately begin play upon world initialization.
     */
    explicit FTestWorld(bool bBeginPlay = false);

    /**
     * Retrieves the game instance managed by the test world.
     *
     * @return A pointer to the currently managed game instance.
     */
    UGameInstance *GetGameInstance() const
    {
        return GameInstance.Get();
    }

    /**
     * Retrieves the world managed by the test world object.
     *
     * @return A pointer to the currently managed world.
     */
    UWorld *GetWorld() const
    {
        return World.Get();
    }

    /**
     * Checks whether the world is currently in play mode.
     *
     * @return True if the world is in play mode, false otherwise.
     */
    bool IsInPlay() const
    {
        return bIsPlay;
    }

    /**
     * Initiates the gameplay sequence for the managed world by setting up actors
     * and marking the world as in play mode. This method should only be called once per world instance.
     * Logs a warning if called multiple times for the same world.
     */
    void BeginPlay();

  private:
    static FGameInstancePtr CreateGameInstance();

    FGameInstancePtr GameInstance;
    FWorldPtr World;
    TSharedRef<SOverlay> Overlay = SNew(SOverlay);
    TSharedRef<SGameLayerManager> GameLayerManager = SNew(SGameLayerManager);
    bool bIsPlay = false;
};
