// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/TargetedMoveEventPayload.h"
#include "UObject/Object.h"

#include "HitCheckPayload.generated.h"

class IBattler;
class IBattleMove;
/**
 * The struct that holds the actual payload data
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FHitCheckData
{
    GENERATED_BODY()

    /**
     * The invoking move
     */
    UPROPERTY(BlueprintReadOnly, Category = GameplayEvents)
    TScriptInterface<IBattleMove> Move;

    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    TScriptInterface<IBattler> User;

    /**
     * The target of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    TScriptInterface<IBattler> Target;

    /**
     * The base accuracy of the move.
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    int32 BaseAccuracy;

    /**
     * Default constructor
     */
    FHitCheckData() = default;

    /**
     * Construct a new payload from the given data
     * @param Move The invoking move
     * @param User The user of the move
     * @param Target The target of the move
     * @param BaseAccuracy The base accuracy of the move.
     */
    FHitCheckData(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User,
                  const TScriptInterface<IBattler> &Target, int32 BaseAccuracy);
};

/**
 * The event payload for a hit check.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UHitCheckPayload : public UObject, public ITargetedMoveEventPayload
{
    GENERATED_BODY()

  public:
    /**
     * Create a new payload with the provided event data
     * @param Move The invoking move
     * @param User The user of the move
     * @param Target The target of the move
     * @param BaseAccuracy The base accuracy of the move.
     * @return The created payload
     */
    static UHitCheckPayload *Create(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User,
                                    const TScriptInterface<IBattler> &Target, int32 BaseAccuracy);

    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    const FHitCheckData &GetData() const;

    const TScriptInterface<IBattler> &GetUser() const final;
    const TScriptInterface<IBattler> &GetTarget() const final;

    /**
     * Set the base accuracy to the given amount
     * @param Amount The new accuracy amount
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetBaseAccuracy(int32 Amount) const;

  private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FHitCheckData> Data;
};
