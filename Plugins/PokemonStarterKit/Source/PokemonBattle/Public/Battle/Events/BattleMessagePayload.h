// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMessage.h"
#include "RunningMessageSetPayload.h"
#include "UObject/Object.h"

#include "BattleMessagePayload.generated.h"

/**
 * Event payload to use to add battle messages to be displayed later.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UBattleMessagePayload : public UObject, public IRunningMessageSetPayload {
    GENERATED_BODY()

  public:
    const FRunningMessageSet &GetRunningMessageSet() const override;

    /**
     * The messages held inside the payload
     */
    UPROPERTY(BlueprintReadOnly, Category = BattleEvents)
    FRunningMessageSet Messages;
};
