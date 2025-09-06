// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "CommandStack.generated.h"

class UCommand;

/**
 * Wrapper for the commands in the stack of commands that are added
 */
USTRUCT(BlueprintInternalUseOnly)
struct FCommandStackFrame
{
    GENERATED_BODY()

    /**
     * The list of commands being held
     */
    UPROPERTY()
    TArray<TObjectPtr<UCommand>> Commands;

    int32 Index;
};
