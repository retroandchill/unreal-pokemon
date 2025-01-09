// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Utils/Unreachable.h"

#if RETROLIB_WITH_UE5CORO
#include "UE5Coro.h"
#endif

#include "BlueprintCoroutineActionBase.generated.h"

/**
 *
 */
UCLASS(Abstract)
class RETROLIB_API UBlueprintCoroutineActionBase : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

#if RETROLIB_WITH_UE5CORO
  public:
    void Activate() final;

    UWorld *GetWorld() const override;

  protected:
    virtual UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) ABSTRACT_METHOD

public:
    UE5Coro::TCoroutine<> UntilComplete(FForceLatentCoroutine Coro = {}) const;

protected:
    const UObject *GetWorldContext() const {
        return WorldContext;
    }

    void SetWorldContext(const UObject *WorldContextObject) {
        WorldContext = WorldContextObject;
    }
#endif

  private:
    UPROPERTY()
    TObjectPtr<const UObject> WorldContext;
};
