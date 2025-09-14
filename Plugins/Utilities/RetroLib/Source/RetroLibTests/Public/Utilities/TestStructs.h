// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "TestStructs.generated.h"

USTRUCT()
struct FTestStruct
{
    GENERATED_BODY()

    int32 Value1 = 3;

    double Value2 = 4.6;

    TSharedPtr<FString> Value3 = MakeShared<FString>(TEXT("Hello World!"));
};
