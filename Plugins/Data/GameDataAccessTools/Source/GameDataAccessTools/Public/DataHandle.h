// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "DataHandle.generated.h"

/**
 *
 */
USTRUCT(meta = (RecordStruct, UseProperties, ReadOnly))
struct FDataHandleEntry
{
    GENERATED_BODY()

    UPROPERTY(meta = (Required))
    FName ID;

    UPROPERTY(meta = (Required))
    FText DisplayName;
};
