// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battler.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/Actor.h"

#include "BattlerActor.generated.h"

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
    
    TScriptInterface<IBattler> Initialize(const TScriptInterface<IBattleSide> &Side,
                                          const TScriptInterface<IPokemon> &Pokemon,
                                          bool ShowImmediately = false) override;

protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
public:
    
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
    FMainBattleStat GetAttack() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FMainBattleStat GetDefense() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FMainBattleStat GetSpecialAttack() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FMainBattleStat GetSpecialDefense() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FMainBattleStat GetSpeed() const override;

    /**
     * @copydoc IBattler::GetStatStages
     */
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetStatStage(FName Stat) const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    float GetExpPercent() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    TArray<FName> GetTypes() const override;

    UBattlerAbilityComponent* GetAbilityComponent() const override;

    UFUNCTION(BlueprintPure, Category = Items)
    bool IsHoldItemActive() const override;

    UFUNCTION(BlueprintPure, Category = Items)
    const TScriptInterface<IHoldItemBattleEffect> &GetHoldItem() const override;

    UFUNCTION(BlueprintPure, Category = Moves)
    const TArray<TScriptInterface<IBattleMove>> &GetMoves() const override;

    void SelectActions() override;
    uint8 GetActionCount() const override;
    ranges::any_view<TScriptInterface<IBattler>> GetAllies() const override;

    void ShowSprite() const override;

  private:
    /**
     * Spawn the underlying sprite actor into the world
     * @param ShouldShow Is this process being invoked on the initialization of this battler (i.e. a Wild Pokémon)
     */
    void SpawnSpriteActor(bool ShouldShow = false);

    /**
     * The ability component for the battler
     */
    UPROPERTY()
    TObjectPtr<UBattlerAbilityComponent> BattlerAbilityComponent;

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
     * The stats for the given Pokémon in battle
     */
    TMap<FName, int32> StatStages;

    /**
     * The list of gameplay abilities that should be activated immediately upon 
     */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
    TArray<TSubclassOf<UGameplayAbility>> InnateAbilities;

    TArray<FGameplayAbilitySpecHandle> InnateAbilityHandles;

    /**
     * The ability that this battler has
     */
    FGameplayAbilitySpecHandle Ability;

    /**
     * The hold item that this battler has
     */
    UPROPERTY()
    TScriptInterface<IHoldItemBattleEffect> HoldItem;

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
};