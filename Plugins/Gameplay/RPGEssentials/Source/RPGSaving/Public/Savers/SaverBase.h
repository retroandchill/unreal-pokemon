// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "SaverBase.generated.h"

/**
 *
 */
UCLASS(Abstract)
class RPGSAVING_API USaverBase : public UObject
{
    GENERATED_BODY()

  public:
    int32 GetVersion() const
    {
        return Version;
    }

    int32 GetPriority() const
    {
        return Priority;
    }

  protected:
    void SetVersion(const int32 NewVersion)
    {
        Version = NewVersion;
    }

    void SetPriority(const int32 NewPriority)
    {
        Priority = NewPriority;
    }

  private:
    UPROPERTY(EditDefaultsOnly, Getter, Category = "Saving")
    int32 Version = 1;

    UPROPERTY(EditDefaultsOnly, Getter, Category = "Saving")
    int32 Priority = 0;
};
