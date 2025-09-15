// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonUIInputTypes.h"
#include "UObject/Object.h"

#include "CSBindUIActionArgs.generated.h"

/**
 *
 */
USTRUCT(meta = (RecordStruct))
struct UNREALSHARPCOMMONUI_API FCSBindUIActionArgs
{
    GENERATED_BODY()

    FCSBindUIActionArgs() = default;
    explicit(false) FCSBindUIActionArgs(const TSharedRef<FBindUIActionArgs> &InArgs) : Args(InArgs)
    {
    }

    FCSBindUIActionArgs &operator=(const TSharedRef<FBindUIActionArgs> &InArgs)
    {
        Args = InArgs;
        return *this;
    }

    bool IsValid() const
    {
        return Args.IsValid();
    }

    FBindUIActionArgs *GetArgs() const
    {
        return Args.Get();
    }

  private:
    TSharedPtr<FBindUIActionArgs> Args;
};
