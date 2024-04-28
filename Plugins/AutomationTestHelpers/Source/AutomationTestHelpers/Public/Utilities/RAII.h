// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

/**
 * "Deleter" to shutdown the game instance so it doesn't cause issues when it gets garbage collected.
 */
struct FGameInstanceShutdown {
    void operator()(UGameInstance *GameInstance) {
        if (GameInstance != nullptr)
            GameInstance->Shutdown();
    }
};

/**
 * An RAII container for a Game Instance that calls shutdown when the object leaves scope.
 */
using FGameInstancePtr = TUniquePtr<UGameInstance, FGameInstanceShutdown>;

/**
 * "Deleter" to remove a widget from the viewport.
 */
struct FRemoveWidgetFromViewport {
    void operator()(UWidget *Widget) {
        if (Widget != nullptr)
            Widget->RemoveFromParent();
    }
};

/**
 * An RAII container for a Widget that removes it from the viewport when we're done with it.
 */
template <typename T>
    requires std::is_base_of_v<UWidget, T>
using TWidgetPtr = TUniquePtr<T, FRemoveWidgetFromViewport>;