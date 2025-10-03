// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WorldContextExtensions.generated.h"

/**
 * @brief Utility class that extends functionality to manage and set world context in Unreal Engine.
 *
 * UWorldContextExtensions provides static functionality for managing world context
 * by interfacing with the internal UCSManager system. The world context can be used
 * to define the context in which various operations occur within the game.
 */
UCLASS()
class COMMONUTILITIES_API UWorldContextExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * @brief Sets the world context for the current session.
     *
     * This method allows setting a specific UObject as the current world context
     * for various operations in the game. It utilizes the UCSManager system to
     * manage the world context.
     *
     * @param Object The UObject to be set as the current world context.
     */
    UFUNCTION(meta = (ScriptMethod))
    static void SetWorldContext(UObject *Object);
};
