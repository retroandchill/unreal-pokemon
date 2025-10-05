// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "ScopedInstancesExporter.generated.h"

/**
 * @class UScopedInstancesExporter
 * @brief A utility class designed to manage scoped instances within the UnrealSharp testing environment.
 *
 * UScopedInstancesExporter provides functionality for handling and manipulating game instances,
 * including managing players at runtime.
 */
UCLASS()
class UNREALSHARPTESTUTILS_API UScopedInstancesExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Removes the specified player from the game environment.
     *
     * This static function invokes the necessary logic to handle the removal of a player instance
     * during runtime, ensuring the appropriate cleanup for the given ULocalPlayer object.
     *
     * @param Player A pointer to the ULocalPlayer instance that is to be removed.
     */
    UNREALSHARP_FUNCTION()
    static void RemovePlayer(ULocalPlayer *Player);
};
