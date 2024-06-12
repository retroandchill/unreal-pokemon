// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battler.h"
#include "GameFramework/Actor.h"

#include "BattlerActor.generated.h"

class IBattleSide;
class IPokemon;
/**
 * This is the actor that represents the Pokémon in battle. It acts as a bridge between the Pokémon and the rendered sprite.
 */
UCLASS()
class POKEMONBATTLE_API ABattlerActor : public AActor, public IBattler {
    GENERATED_BODY()

public:
    TScriptInterface<IBattler> Initialize(const TScriptInterface<IBattleSide> &Side,
                                          const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately = false) override;
    FGuid GetInternalId() const override;
    
    UFUNCTION(BlueprintPure, Category = Context)
    TScriptInterface<IBattleSide> GetOwningSide() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    FText GetNickname() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetPokemonLevel() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetAttack() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetDefense() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetSpecialAttack() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetSpecialDefense() const override;
    
    UFUNCTION(BlueprintPure, Category = Stats)
    TArray<FName> GetTypes() const override;
    
    UFUNCTION(BlueprintPure, Category = Ability)
    bool IsAbilityActive() const override;
    
    UFUNCTION(BlueprintPure, Category = Ability)
    const TScriptInterface<IAbilityBattleEffect> & GetAbility() const override;
    
    UFUNCTION(BlueprintPure, Category = Items)
    bool IsHoldItemActive() const override;
    
    UFUNCTION(BlueprintPure, Category = Items)
    const TScriptInterface<IHoldItemBattleEffect> & GetHoldItem() const override;
    
    UFUNCTION(BlueprintPure, Category = Moves)
    const TArray<TScriptInterface<IBattleMove>> & GetMoves() const override;
    
    void SelectActions() const override;
    uint8 GetActionCount() const override;
    void ForEachAlly(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const override;
    void ForEachBattleEffect(const TFunctionRef<void(const TScriptInterface<IBattlerEffect> &)> &Callback) const override;

private:
    /**
     * Spawn the underlying sprite actor into the world
     * @param ShouldShow Is this process being invoked on the initialization of this battler (i.e. a Wild Pokémon)
     */
    void SpawnSpriteActor(bool ShouldShow = false);
    
    FGuid InternalId;

    TWeakInterfacePtr<IBattleSide> OwningSide;

    /**
     * The actual class used for the battler's sprite.
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes", meta = (MustImplement = "BattlerSprite"))
    TSoftClassPtr<AActor> BattlerSpriteClass;
    
    UPROPERTY()
    TScriptInterface<IPokemon> WrappedPokemon;

    UPROPERTY()
    TScriptInterface<IAbilityBattleEffect> Ability;

    UPROPERTY()
    TScriptInterface<IHoldItemBattleEffect> HoldItem;

    UPROPERTY()
    TArray<TScriptInterface<IBattleMove>> Moves;
    
};