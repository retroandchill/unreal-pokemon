// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMoveFunctionCode.h"
#include "UObject/Object.h"
#include "TargetSuccessCheckPayload.generated.h"

class IBattleMove;

USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FTargetSuccessCheckPayload {
    GENERATED_BODY()

    /**
     * The invoking move
     */
    UPROPERTY(BlueprintReadOnly, Category = GameplayEvents)
    TScriptInterface<IBattleMove> Move;

    /**
     * Any messages to be down after processing.
     */
    UPROPERTY(BlueprintReadWrite, Category = GameplayEvents)
    FRunningMessageSet Messages;

    /**
     * Should messages be shown?
     */
    UPROPERTY(BlueprintReadOnly, Category = GameplayEvents)
    bool bShowMessages = true;

    /**
     * Does the move succeed?
     */
    UPROPERTY(BlueprintReadWrite, Category = GameplayEvents)
    bool bSuccess = true;

    /**
     * Default constructor
     */
    FTargetSuccessCheckPayload();

    /**
     * Construct a new payload from the given data
     * @param Move The invoking move
     * @param Messages Any messages to be down after processing.
     * @param bShowMessages Should messages be shown?
     */
    FTargetSuccessCheckPayload(const TScriptInterface<IBattleMove> &Move, const FRunningMessageSet& Messages, bool bShowMessages = true);
    
};

/**
 * Payload object for performing a success check against a target
 */
UCLASS()
class POKEMONBATTLE_API UTargetSuccessCheckPayload : public UObject {
    GENERATED_BODY()

public:
    /**
     * Create a new payload with the provided event data
    ** @param Move The invoking move
     * @param Messages Any messages to be down after processing.
     * @param bShowMessages Should messages be shown?
     * @return The created payload
     */
    static UTargetSuccessCheckPayload* Create(const TScriptInterface<IBattleMove> &Move, const FRunningMessageSet& Messages, bool bShowMessages = true);
    
    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    FTargetSuccessCheckPayload& GetData() const;

private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FTargetSuccessCheckPayload> Data;

};
