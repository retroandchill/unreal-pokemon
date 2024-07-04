// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMove.h"
#include "GameplayAbilitySpecHandle.h"

#include "PokemonBattleMove.generated.h"

class UGameplayAbility;
class UBattleMoveFunctionCode;
class UMoveEffectContext;
struct FDamageMultipliers;
struct FModifiedDamage;

/**
 * The base class used for all battle moves used by the game.
 */
UCLASS(Blueprintable)
class POKEMONBATTLE_API UPokemonBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

  public:
    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattler>& Battler,
                                             const TScriptInterface<IMove> &Move) override;
    
    UFUNCTION(BlueprintPure, Category = Usability)
    bool IsUsable() const override;
    
    TArray<TScriptInterface<IBattler>> GetAllPossibleTargets() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    FText GetDisplayName() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    int32 GetCurrentPP() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    int32 GetMaxPP() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    FName GetDisplayType() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    int32 GetBasePower() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    int32 GetAccuracy() const override;
    
    UFUNCTION(BlueprintPure, Category = Display)
    virtual EMoveDamageCategory GetCategory() const override;
    
    UFUNCTION(BlueprintPure, Category = Usability)
    const FMoveTarget &GetTargetType() const override;
    
    UFUNCTION(BlueprintPure, Category = Metadata)
    const TArray<FName> &GetTags() const override;
    
    UFUNCTION(BlueprintPure, Category = Usability)
    int32 GetPriority() const override;

    UFUNCTION(BlueprintPure, Category = Effects)
    int32 GetAdditionalEffectChance() const override;
    
    UFUNCTION(BlueprintCallable, Category = Usability)
    void PayCost(int32 Amount) override;
    
    UFUNCTION(BlueprintPure, Category = Context)
    const TScriptInterface<IBattler> &GetOwningBattler() const override;

    FGameplayAbilitySpecHandle TryActivateMove(const TArray<TScriptInterface<IBattler>> &Targets) override;

private:
    /**
     * The current battle that is ongoing
     */
    UPROPERTY()
    TScriptInterface<IBattler> Owner;

    /**
     * The move that is actively being wrapped around
     */
    UPROPERTY()
    TScriptInterface<IMove> WrappedMove;

    /**
     * The ability spec for this function code on the battler.
     */
    FGameplayAbilitySpecHandle FunctionCode;
};