// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

/**
 * "Deleter" to shutdown the game instance so it doesn't cause issues when it gets garbage collected.
 */
struct FGameInstanceShutdown
{
    void operator()(UGameInstance *GameInstance)
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
 * "Deleter" to remove a widget from the viewport.
 */
struct FRemoveWidgetFromViewport
{
    void operator()(UWidget *Widget)
    {
        if (Widget != nullptr)
        {
            Widget->RemoveFromParent();
        }
    }
};

/**
 * An RAII container for a Widget that removes it from the viewport when we're done with it.
 */
template <typename T>
    requires std::is_base_of_v<UWidget, T>
using TWidgetPtr = TUniquePtr<T, FRemoveWidgetFromViewport>;

/**
 * "Deleter" used to clean up the world
 */
struct FCleanupWorld
{
    void operator()(UWorld *World)
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
 * "Deleter" used to remove a local player
 */
struct FRemovePlayer
{
    void operator()(ULocalPlayer *Player)
    {
        if (Player != nullptr)
        {
            Player->PlayerRemoved();
        }
    }
};

/**
 * Smart pointer to the world that calls cleanup when it gets destroyed.
 */
using FPlayerPtr = TUniquePtr<ULocalPlayer, FRemovePlayer>;