// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WorldContextExtensions.generated.h"

/**
 *
 */
UCLASS()
class COMMONUTILITIES_API UWorldContextExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static void SetWorldContext(UObject *Object);
};
