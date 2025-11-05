// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battler.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpecHandle.h"

#include "BattlerActor.generated.h"

class UTurnBasedEffectComponent;
class ITrainer;
class UGameplayEffect;
struct FOnAttributeChangeData;
class UGameplayAbility;
class UBattlerAbilityComponent;
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
    /**
     * Initialize the default object for the game
     */
    ABattlerActor();

    UE5Coro::TCoroutine<TScriptInterface<IBattler>> Initialize(TScriptInterface<IBattleSide> Side,
                                                               TScriptInterface<IPokemon> Pokemon,
                                                               bool ShowImmediately = false,
                                                               FForceLatentCoroutine = {}) override;

  protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  public:
    UFUNCTION(BlueprintPure, Category = Metadata)
    FGuid GetInternalId() const override;

    UFUNCTION(BlueprintPure, Category = Context)
    const TScriptInterface<IBattleSide> &GetOwningSide() const override;

    UFUNCTION(BlueprintPure, Category = Context)
    const TScriptInterface<IPokemon> &GetWrappedPokemon() const override;

    UFUNCTION(BlueprintPure, Category = Context)
    bool IsActive() const override;

    const FSpeciesData &GetSpecies() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FText GetNickname() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    EPokemonGender GetGender() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetPokemonLevel() const override;

    UFUNCTION(BlueprintCallable, Category = Stats)
    void RefreshStats() override;

    UFUNCTION(BlueprintPure, DisplayName = "Get HP Percent", Category = Stats)
    float GetHPPercent() const override;

    void TakeBattleDamage(int32 Damage) override;

    UFUNCTION(BlueprintPure, Category = Stats)
    bool IsFainted() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    bool IsNotFainted() const override;

    UFUNCTION(BlueprintCallable, Category = Visuals)
    void Faint() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    bool CanGainExp() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    float GetExpPercent() const override;

    UE5Coro::TCoroutine<TArray<FExpGainInfo>> GiveExpToParticipants() override;

    UE5Coro::TCoroutine<FLevelUpStatChanges> GainExpAndEVs(int32 Exp, const TMap<FName, uint8> &EVs) override;

    UFUNCTION(BlueprintPure, Category = Stats)
    TArray<FName> GetTypes() const override;

    UFUNCTION(BlueprintPure, Category = GameplayAbilities)
    UBattlerAbilityComponent *GetAbilityComponent() const override;

    UFUNCTION(BlueprintPure, Category = GameplayAbilities)
    UTurnBasedEffectComponent *GetTurnBasedEffectComponent() const override;

    UFUNCTION(BlueprintPure, Category = Moves)
    const TArray<TScriptInterface<IBattleMove>> &GetMoves() const override;

    UFUNCTION(BlueprintPure, Category = Switching)
    FText GetRecallMessage() const;

    UE5Coro::TCoroutine<> PerformSwitch(const TScriptInterface<IBattler> &SwitchTarget) override;

    UFUNCTION(BlueprintPure, Category = Ownership)
    bool IsOwnedByPlayer() const;

    void SelectActions() override;
    void RequireSwitch() override;
    uint8 GetActionCount() const override;
    bool CanSelectActions() const override;

    Retro::TGenerator<TScriptInterface<IBattler>> GetAllies() const override;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Visuals, meta = (AutoCreateRefTerm = Offset))
    void ShowSprite(const FVector &Offset) const override;

    UFUNCTION(BlueprintCallable, Category = Visuals)
    void HideSprite() const override;

    void RecordParticipation() override;
    void AddParticipant(const TScriptInterface<IBattler> &Participant) override;

    UFUNCTION(BlueprintCallable, Category = "Battle|TurnFlow")
    int32 GetTurnCount() const override;

    const TOptional<FStatusEffectInfo> &GetStatusEffect() const override;

    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    void InflictStatusEffect(FStatusHandle StatusEffectID, FActiveGameplayEffectHandle EffectHandle) override;

    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    void CureStatusEffect() override;

    UFUNCTION(BlueprintImplementableEvent, Category = Moves)
    void OnMoveFailed(const TScriptInterface<IBattleMove> &Move) override;

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    FText GetMessageOnRecall() const;

    UFUNCTION(BlueprintNativeEvent, Category = Visuals)
    FTransform GetSpriteTransform() const;

  private:
    /**
     * Take a change in the Battler's HP and propagate that to the base HP as needed.
     * @param Data The data about the HP change
     */
    void UpdateHPValue(const FOnAttributeChangeData &Data) const;

    /**
     * Spawn the underlying sprite actor into the world
     * @param ShouldShow Is this process being invoked on the initialization of this battler (i.e. a Wild Pokémon)
     */
    virtual UE5Coro::TCoroutine<> SpawnSpriteActor(bool ShouldShow = false);

    /**
     * The ability component for the battler
     */
    UPROPERTY()
    TObjectPtr<UBattlerAbilityComponent> BattlerAbilityComponent;

    UPROPERTY()
    TObjectPtr<UTurnBasedEffectComponent> TurnBasedEffectComponent;

    /**
     * The internal ID of the battler
     */
    FGuid InternalId;

    /**
     * The side that owns this battler
     */
    UPROPERTY()
    TScriptInterface<IBattleSide> OwningSide;

    UPROPERTY()
    TSet<FGuid> Participants;

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

    UPROPERTY()
    TScriptInterface<ITrainer> OwningTrainer;

    /**
     * The list of gameplay abilities that should be activated immediately upon the actor being spawned.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
    TArray<TSubclassOf<UGameplayAbility>> InnateAbilities;

    /**
     * The handles for the Pokémon's innate abilities that have been created
     */
    TArray<FGameplayAbilitySpecHandle> InnateAbilityHandles;

    /**
     * The list of gameplay effects that should be activated immediately upon the actor being spawned.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
    TArray<TSubclassOf<UGameplayEffect>> InnateEffects;

    /**
     * Handles for the Pokémon's innate effects that have been created
     */
    TArray<FActiveGameplayEffectHandle> InnateEffectHandles;

    /**
     * The ability that this battler has
     */
    FGameplayAbilitySpecHandle Ability;

    /**
     * The item the battler is holding
     */
    FGameplayAbilitySpecHandle HoldItem;

    /**
     * The status effect that battler has
     */
    TOptional<FStatusEffectInfo> StatusEffect;

    FGameplayAbilitySpecHandle SwitchActionHandle;

    /**
     * The moves this battler knows
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

    int32 TurnCount = 0;
};