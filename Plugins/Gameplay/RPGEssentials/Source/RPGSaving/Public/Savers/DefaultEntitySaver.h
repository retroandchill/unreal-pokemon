// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EntitySaver.h"

#include "DefaultEntitySaver.generated.h"

/**
 *
 */
UCLASS()
class RPGSAVING_API UDefaultEntitySaver : public UEntitySaver
{
    GENERATED_BODY()

  protected:
    UDefaultEntitySaver()
    {
        SetPriority(std::numeric_limits<int32>::min());
    }

    bool NativeSupports(TSubclassOf<URPGEntity> Entity) const override
    {
        return true;
    }
};
