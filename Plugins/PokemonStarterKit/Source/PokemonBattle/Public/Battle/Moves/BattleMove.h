// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Moves/MoveDamageCategory.h"
#include "UObject/Interface.h"
#include <range/v3/view/any_view.hpp>

#include "BattleMove.generated.h"

struct FMoveTarget;
struct FType;
class IMove;
class IBattler;
class IBattle;

struct POKEMONBATTLE_API FTargetWithIndex {
    TWeakInterfacePtr<IBattler> Target;
    int32 BattlerIndex;

    explicit FTargetWithIndex(const TScriptInterface<IBattler> &Battler);

    TScriptInterface<IBattler> SwapIfNecessary() const;
};

// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class POKEMONBATTLE_API UBattleMove : public UInterface {
    GENERATED_BODY()
};

/**
 * Wrapper around a move for its effects in battle.
 */
class POKEMONBATTLE_API IBattleMove {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Initialize the move from the given owned move.
     * @param Battler
     * @param Move The move that is currently owned by the user.
     * @return The initialized interface
     */
    virtual TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattler> &Battler,
                                                     const TScriptInterface<IMove> &Move) = 0;

    /**
     * Compute all possible targets for the move based on the given user information
     * @return The found list of targets
     */
    virtual ranges::any_view<TScriptInterface<IBattler>> GetAllPossibleTargets() const = 0;

    /**
     * Is the move usable
     * @return Is the move usable
     */
    UFUNCTION(BlueprintCallable, Category = Usability)
    virtual bool IsUsable() const = 0;

    /**
     * Get the display name for the move
     * @return The name to show to the user
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual FText GetDisplayName() const = 0;

    UFUNCTION(BlueprintCallable, Category = Display)
    virtual int32 GetCurrentPP() const = 0;

    UFUNCTION(BlueprintCallable, Category = Display)
    virtual int32 GetMaxPP() const = 0;

    /**
     * Get the display type for the move
     * @return The type to show to the user
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual FName GetDisplayType() const = 0;

    /**
     * Get the category of the move's damage capabilities
     */
    UFUNCTION(BlueprintCallable, Category = Context)
    virtual EMoveDamageCategory GetCategory() const = 0;

    UFUNCTION(BlueprintCallable, Category = Display)
    virtual int32 GetBasePower() const = 0;

    UFUNCTION(BlueprintCallable, Category = Display)
    virtual int32 GetAccuracy() const = 0;

    /**
     * Get the category of the move's damage capabilities
     */
    UFUNCTION(BlueprintCallable, Category = Usability)
    virtual const FMoveTarget &GetTargetType() const = 0;

    /**
     * Get the priority for the move
     * @return The priority of the move
     */
    UFUNCTION(BlueprintCallable, Category = Usability)
    virtual int32 GetPriority() const = 0;

    /**
     * Get the move's additional effect chance
     * @return The move's additional effect chance
     */
    UFUNCTION(BlueprintCallable, Category = Effects)
    virtual int32 GetAdditionalEffectChance() const = 0;

    /**
     * Get the category of the move's damage capabilities
     */
    UFUNCTION(BlueprintCallable, Category = Metadata)
    virtual const TArray<FName> &GetTags() const = 0;

    /**
     * Pay the move's PP cost to use
     */
    UFUNCTION(BlueprintCallable, Category = Usability)
    virtual void PayCost(int32 Amount = 1) = 0;

    /**
     * Get the active battle that owns this one
     * @return The current owning battle for this move
     */
    UFUNCTION(BlueprintCallable, Category = Context)
    virtual const TScriptInterface<IBattler> &GetOwningBattler() const = 0;

    /*
     * Try to activate the move returning the spec handle for the move execution
     */
    virtual FGameplayAbilitySpecHandle TryActivateMove(const TArray<FTargetWithIndex> &Targets) = 0;
};