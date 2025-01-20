// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Ranges/Views/AnyView.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "Battle.generated.h"

class IBattleHUD;
struct FBattleInfo;
class IAbilityDisplayComponent;
struct FGameplayEffectSpecHandle;
class IBattleSide;
class IBattleAction;
class IAbilityBattleEffect;
class IFieldEffect;
class IBattler;

/**
 * Enum for the result of the battle in question
 */
UENUM(BlueprintType)
enum class EBattleResult : uint8 {
    /**
     * The player won the battle in question
     */
    Victory,

    /**
     * The player was defeated in battle
     */
    Defeat,

    /**
     * The battle ended in a draw
     */
    Draw,

    /**
     * The player's side fled from battle
     */
    PlayerEscaped,

    /**
     * The opponent's side fled from battle
     */
    FoeEscaped,

    /**
     * The battle ended early (typically for event purposes)
     */
    Inconclusive
};

/**
 * Delegate called when the battle ends
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleEnd, EBattleResult)

    // This class does not need to be modified.
    UINTERFACE(BlueprintType) class POKEMONBATTLE_API UBattle : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents an active battle that is occurring.
 */
class POKEMONBATTLE_API IBattle {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    static constexpr int32 PlayerSideIndex = 0;
    static constexpr int32 OpponentSideIndex = 1;

    virtual TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) = 0;

    /**
     * Initialize a new battle with the given battle information
     * @param BattleInfo ThFSideWithIndexCallbacke information about the battle to use when initializing the battle
     * @return A reference to this object
     */
    virtual UE5Coro::TCoroutine<TScriptInterface<IBattle>> Initialize(const FBattleInfo &BattleInfo) = 0;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle|State")
    bool IsTrainerBattle() const;

    /**
     * Have the player take possession of the battle pawn and begin the battle intro.
     * @param PlayerController The player controller to shift control over to the battle pawn
     * @param
     */
    virtual UE5Coro::TCoroutine<EBattleResult> ConductBattle(APlayerController *PlayerController,
                                                             FForceLatentCoroutine = {}) = 0;

    /**
     * This is to be called after all pre-battle setup has been completed (i.e. intro animations, sending out Pokémon,
     * etc.)
     */
    virtual UE5Coro::TCoroutine<> StartBattle() = 0;

    virtual UE5Coro::TCoroutine<>
    OnBattlersEnteringBattle(Retro::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers) = 0;

    /**
     * Add an action to the pending queue
     * @param Action The action to take
     */
    virtual void QueueAction(TUniquePtr<IBattleAction> &&Action) = 0;

    /**
     * Check if action selection is completed
     * @return Is action selection done
     */
    virtual bool ActionSelectionFinished() const = 0;

    /**
     * Get the battle pawn used in battle
     * @return The pawn used in battle
     */
    virtual APawn *GetBattlePawn() const = 0;

    virtual const TScriptInterface<IBattleHUD> &GetBattleHUD() const = 0;

    virtual const TScriptInterface<IBattleSide> &GetPlayerSide() const = 0;
    virtual const TScriptInterface<IBattleSide> &GetOpposingSide() const = 0;

    /**
     * Get all sides in the current battle
     * @return A view of all sides in the battle
     */
    virtual Retro::TGenerator<TScriptInterface<IBattleSide>> GetSides() const = 0;

    /**
     * Get all active battlers in the battle
     * @return A view of all active battlers
     */
    virtual Retro::TGenerator<TScriptInterface<IBattler>> GetActiveBattlers() const = 0;

    /**
     * Initiate the process of selecting actions for the given battler.
     */
    virtual void BeginActionSelection(const TScriptInterface<IBattler> &Battler) = 0;

    virtual void PromptMandatorySwitch(const TScriptInterface<IBattler> &Battler) = 0;

    /**
     * Execute the bound action in battle
     * @param Action The action to execute
     * @param
     */
    virtual UE5Coro::TCoroutine<> ExecuteAction(IBattleAction &Action, FForceLatentCoroutine = {}) = 0;

    /**
     * Take the current battler and check if the player is able to run from battle
     * @param Battler The battler that will be used to perform the run calculation
     * @param bDuringBattle Is this being called during the send out prompt?
     * @return Can the player run?
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle|Flow")
    bool RunCheck(const TScriptInterface<IBattler> &Battler, bool bDuringBattle);

    /**
     * Terminate the battle with the specified result
     * @param Result The result for the battle state
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Battle|Flow")
    void EndBattle(EBattleResult Result);
};