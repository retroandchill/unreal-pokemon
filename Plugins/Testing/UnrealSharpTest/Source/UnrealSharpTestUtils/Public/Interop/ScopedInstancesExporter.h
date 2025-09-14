// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "ScopedInstancesExporter.generated.h"

/**
 *
 */
UCLASS()
class UNREALSHARPTESTUTILS_API UScopedInstancesExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static void RemovePlayer(ULocalPlayer *Player);
};
