// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battler.h"
#include "GameFramework/Actor.h"
#include "Battle/Abilities/AbilityBattleEffect.h"

#include "BattlerActor.generated.h"

class IBattlerController;
class IBattlerSprite;
class IBattleSide;
class IPokemon;
/**
 * This is the actor that represents the Pokémon in battle. It acts as a bridge between the Pokémon and the rendered
 * sprite.
 */
UCLASS()
class POKEMONBATTLE_API ABattlerActor : public AActor, public IBattler {
    GENERATED_BODY()

  public:
    TScriptInterface<IBattler> Initialize(const TScriptInterface<IBattleSide> &Side,
                                          const TScriptInterface<IPokemon> &Pokemon,
                                          bool ShowImmediately = false) override;
    FGuid GetInternalId() const override;

    UFUNCTION(BlueprintPure, Category = Context)
    const TScriptInterface<IBattleSide> &GetOwningSide() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FText GetNickname() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    EPokemonGender GetGender() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetPokemonLevel() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetHP() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetMaxHP() const override;

    UFUNCTION(BlueprintPure, DisplayName = "Get HP Percent", Category = Stats)
    float GetHPPercent() const override;

    void TakeBattleDamage(int32 Damage) override;

    UFUNCTION(BlueprintPure, Category = Stats)
    bool IsFainted() const override;

    UFUNCTION(BlueprintCallable, Category = Visuals)
    void Faint() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetAttack() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetDefense() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetSpecialAttack() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetSpecialDefense() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetSpeed() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    float GetExpPercent() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    TArray<FName> GetTypes() const override;

    UFUNCTION(BlueprintPure, Category = Ability)
    bool IsAbilityActive() const override;

    UFUNCTION(BlueprintPure, Category = Ability)
    UAbilityBattleEffect* GetAbility() const override;

    UFUNCTION(BlueprintPure, Category = Items)
    bool IsHoldItemActive() const override;

    UFUNCTION(BlueprintPure, Category = Items)
    const TScriptInterface<IHoldItemBattleEffect> &GetHoldItem() const override;

    UFUNCTION(BlueprintPure, Category = Moves)
    const TArray<TScriptInterface<IBattleMove>> &GetMoves() const override;

    void SelectActions() override;
    uint8 GetActionCount() const override;
    void ForEachAlly(TInterfaceCallback<IBattler> Callback) const override;
    void ForEachBattleEffect(TInterfaceCallback<IBattlerEffect> Callback) const override;
    void ForEachIndividualTraitHolder(TInterfaceCallback<IIndividualTraitHolder> Callback) const override;
    bool ForAnyIndividualTraitHolder(const TFunctionRef<bool(const IIndividualTraitHolder&)> Predicate) const override;

    void ShowSprite() const override;

  private:
    /**
     * Spawn the underlying sprite actor into the world
     * @param ShouldShow Is this process being invoked on the initialization of this battler (i.e. a Wild Pokémon)
     */
    void SpawnSpriteActor(bool ShouldShow = false);

    /**
     * The internal ID of the battler
     */
    FGuid InternalId;

    /**
     * The side that owns this battler
     */
    UPROPERTY()
    TScriptInterface<IBattleSide> OwningSide;

    /**
     * The actual class used for the battler's sprite.
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes", meta = (MustImplement = "BattlerSprite"))
    TSoftClassPtr<AActor> BattlerSpriteClass;

    /**
     * The Pokémon that this battler wraps around
     */
    UPROPERTY()
    TScriptInterface<IPokemon> WrappedPokemon;

    /**
     * The ability that this battler has
     */
    FManagedBattleAbility Ability;

    /**
     * The hold item that this battler has
     */
    UPROPERTY()
    TScriptInterface<IHoldItemBattleEffect> HoldItem;

    /**
     * The moves this battler knowns
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattleMove>> Moves;

    /**
     * The actor that creates the battler's sprite
     */
    UPROPERTY()
    TObjectPtr<AActor> Sprite;

    /**
     * The actor that serves as the controller for selecting actions.
     */
    UPROPERTY()
    TScriptInterface<IBattlerController> Controller;
    
};