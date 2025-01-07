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

protected:
    virtual UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine Coro = {}) ABSTRACT_METHOD
#endif

};
