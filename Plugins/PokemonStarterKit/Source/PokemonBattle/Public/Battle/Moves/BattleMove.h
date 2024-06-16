// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleDamage.h"
#include "UObject/Interface.h"

#include "BattleMove.generated.h"

struct FType;
class IMove;
class IBattler;
class IBattle;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
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
     * @param Battle
     * @param Move The move that is currently owned by the user.
     * @return The initialized interface
     */
    virtual TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattle> &Battle,
                                                     const TScriptInterface<IMove> &Move) = 0;

    /**
     * Compute all possible targets for the move based on the given user information
     * @param User The user of the move in question
     * @return The found list of targets
     */
    UFUNCTION(BlueprintNativeEvent, Category = Usability)
    TArray<TScriptInterface<IBattler>> GetAllPossibleTargets(const TScriptInterface<IBattler> &User) const;

    UFUNCTION(BlueprintNativeEvent, Category = Usability)
    bool IsUsable() const;

    /**
     * Get the display name for the move
     * @return The name to show to the user
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    FText GetDisplayName() const;

    UFUNCTION(BlueprintNativeEvent, Category = Display)
    int32 GetCurrentPP() const;

    UFUNCTION(BlueprintNativeEvent, Category = Display)
    int32 GetMaxPP() const;

    /**
     * Get the display type for the move
     * @return The type to show to the user
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    FName GetDisplayType() const;

    /**
     * Get the priority for the move
     * @return The priority of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    int32 GetPriority() const;

    /**
     * Pay the move's PP cost to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    void PayCost();

    /**
     * Get the active battle that owns this one
     * @return The current owning battle for this move
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Context)
    TScriptInterface<IBattle> GetOwningBattle() const;

    /**
     * Is this attack the one from the Confusion effect?
     * @return Is this the typeless confusion attack
     */
    virtual bool IsConfusionAttack() const = 0;

    /**
     * Does the move possess the given tag?
     * @param Tag The tag to check for
     * @return Does the move have the given tag
     */
    virtual bool HasTag(FName Tag) const = 0;

    /**
     * Roll to see if this move hits or not.
     * @param User The user of the move.
     * @param Target The target the move is being hit by.
     * @return Did the move successfully hit its target
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Success Checking")
    bool PerformHitCheck(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target);

    /**
     * Calculate the total damage the move will deal.
     * @param User The user of the move.
     * @param Target The target the move is being hit by.
     * @param TargetCount The number of targets being hit.
     * @return The amount of damage dealt
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FBattleDamage CalculateDamage(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                  int32 TargetCount = 1);
};