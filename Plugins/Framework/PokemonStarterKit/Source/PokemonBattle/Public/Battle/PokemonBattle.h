// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle.h"
#include "Battle/Actions/BattleAction.h"
#include "Containers/Queue.h"
#include "Effects/TurnBasedGameplayEffect.h"
#include "RetroLib/Ranges/Views/AnyView.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "Templates/UniquePtr.h"
#include "UObject/Object.h"

#include "PokemonBattle.generated.h"

class IBattleHUD;
class IPokemonCoroutineDispatcher;
class IBattleAnimation;
class UTurnBasedEffectComponent;
struct FGameplayTag;
class UGameplayAbilityDisplayComponent;
class UBattleAbilitySystemComponent;
struct FActionResult;
class IBattleSide;
class ITrainer;

/**
 * Enum for the various phases of battle
 */
UENUM(BlueprintType)
enum class EBattlePhase : uint8 {
    /**
     * The battle is still being set up
     */
    Setup,

    /**
     * Moves are currently being selected
     */
    Selecting,

    /**
     * Actions are currently being executed
     */
    Actions,

    /**
     * The field is being evaluated to check if an outcome can be decided or if the battle needs to go on for another
     * turn
     */
    Judging,

    /**
     * The battle has been decided, tear down must now take place
     */
    Decided
};

/**
 * The actual battle class used for battle flow.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API APokemonBattle : public AActor, public IBattle {
    GENERATED_BODY()

  public:
    APokemonBattle();

    TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) override;
    UE5Coro::TCoroutine<TScriptInterface<IBattle>> Initialize(FBattleInfo BattleInfo,
                                                              FForceLatentCoroutine = {}) override;

  protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool IsTrainerBattle_Implementation() const override;
    UE5Coro::TCoroutine<EBattleResult> ConductBattle(APlayerController *PlayerController,
                                                     FForceLatentCoroutine = {}) override;

  private:
    UE5Coro::TCoroutine<> DisplaySideSendOutAnimation(int32 Index);
    UE5Coro::TCoroutine<EBattleResult> MainBattleLoop();

  public:
    UE5Coro::TCoroutine<> StartBattle() override;

    UE5Coro::TCoroutine<> OnBattlersEnteringBattle(Retro::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers);

    void QueueAction(TUniquePtr<IBattleAction> &&Action) override;
    bool ActionSelectionFinished() const override;

#if WITH_EDITOR
    /**
     * Get the list of actions that are to be queued up and executed. Mainly used for logging as needed.
     * @return The list of actions
     */
    const TArray<TUniquePtr<IBattleAction>> &GetActions() const;

    /**
     * Get the queue of actions that about to or currently being executed. Mainly used for logging as needed.
     * @return The queue of actions
     */
    const TQueue<TUniquePtr<IBattleAction>> &GetActionQueue() const;
#endif

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Battle|Visuals")
    APawn *GetBattlePawn() const final;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<IBattleHUD> &GetBattleHUD() const override {
        return BattleHUD;
    }

    UFUNCTION(BlueprintPure, Category = "Battle|Content")
    const TScriptInterface<IBattleSide> &GetPlayerSide() const override;

    UFUNCTION(BlueprintPure, Category = "Battle|Content")
    const TScriptInterface<IBattleSide> &GetOpposingSide() const override;

    Retro::TGenerator<TScriptInterface<IBattleSide>> GetSides() const override;
    Retro::TGenerator<TScriptInterface<IBattler>> GetActiveBattlers() const override;
    UE5Coro::TCoroutine<> ExecuteAction(IBattleAction &Action, FForceLatentCoroutine = {}) override;

    void BeginActionSelection(const TScriptInterface<IBattler> &Battler) override;
    void PromptMandatorySwitch(const TScriptInterface<IBattler> &Battler) override;

  protected:
    bool RunCheck_Implementation(const TScriptInterface<IBattler> &Battler, bool bDuringBattle) override;

    /**
     * Get the position of the player's side of the field
     * @return The player's primary spawn position
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    FTransform GetPlayerSidePosition() const;

    /**
     * Get the position of the opponent's side of the field
     * @return The opponents primary spawn position
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    FTransform GetOpponentSidePosition() const;

    /**
     * Play the intro sequence for the battle. This sequence ends upon calling DisplayBattleIntroMessage from the
     * Blueprint Graph.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    TScriptInterface<IBattleAnimation> GetBattleIntro();

    /**
     * Display the intro message for the battle.
     */
    UFUNCTION(BlueprintPure, Category = "Battle|Flow")
    FText GetBattleIntroMessage() const;

    /**
     * Display the message for a Pokémon send out prior to the animation
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    TScriptInterface<IBattleAnimation> GetOpponentSendOutAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    TScriptInterface<IBattleAnimation> GetPlayerSendOutAnimation();

    /**
     * Create the HUD used for the battle system
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    TScriptInterface<IBattleHUD> CreateBattleHUD();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    TScriptInterface<IBattleAnimation> GetBattleEndAnimation(EBattleResult Result);

    /**
     * Refresh the battle HUD so that it is up to date
     */
    void RefreshBattleHUD();

    /**
     * Process the result of the battle and exit
     * @param Result The outcome of the battle in question
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Flow")
    TScriptInterface<IBattleAnimation> GetBattleResultAnimation(EBattleResult Result);

    void EndBattle_Implementation(EBattleResult Result) override;

  private:
    void ProcessTurnDurationTrigger(ETurnDurationTrigger Trigger);

    /**
     * Run at the head of every turn. Increments the turn count and initiates action selection.
     */
    UE5Coro::TCoroutine<TOptional<int32>> ProcessTurn();

  protected:
    /**
     * Run all checks that need to be handled at the end of the turn
     */
    UE5Coro::TCoroutine<TOptional<int32>> EndTurn();

    void ClearActionSelection();

  private:
    /**
     * Process all of the actions about to be performed
     */
    UE5Coro::TCoroutine<> ActionProcessing();

    /**
     * Proceed to the next action in the queue
     */
    void NextAction();

    /**
     * Decide the battle for the side with the given index
     * @param SideIndex The index for the battle side that cannot continue
     */
    UE5Coro::TCoroutine<EBattleResult> DecideBattle(int32 SideIndex);

    /**
     * The ability system component used for this actor
     */
    UPROPERTY()
    TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UTurnBasedEffectComponent> TurnBasedEffectComponent;

    /**
     * The current turn number that we're on in battle.
     */
    uint32 TurnCount = 0;

    bool bSwitchPrompting = false;

    UPROPERTY()
    TScriptInterface<IPokemonCoroutineDispatcher> Dispatcher;

    /**
     * The list of sides in battle
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattleSide>> Sides;

    /**
     * The class used to constructing the sides of the battle.
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes", meta = (MustImplement = "BattleSide"))
    TSoftClassPtr<AActor> BattleSideClass;

    /**
     * Mutex object used to lock the action queue while it is being edited.
     */
    FCriticalSection ActionMutex;

    /**
     * Collection used to serve as the holding container for pending.
     */
    TArray<TUniquePtr<IBattleAction>> SelectedActions;

    /**
     * The current number of actions that have been submitted
     */
    TMap<FGuid, uint8> CurrentActionCount;

    /**
     * The number of actions we expect before proceeding to action execution
     */
    TMap<FGuid, uint8> ExpectedActionCount;

    TUniquePtr<TPromise<int32>> ActionsCompletePromise = MakeUnique<TPromise<int32>>();

    /**
     * The actual queue of actions to be executed
     */
    TQueue<TUniquePtr<IBattleAction>> ActionQueue;

    bool bActionTextDisplayed = false;

    /**
     * This is the pawn to take control of when the battle is initiated.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetBattlePawn, Category = "Battle|Context")
    TObjectPtr<APawn> BattlePawn;

    /**
     * A reference to the current player pawn so that control can be relinquished to that pawn after battle is complete
     */
    UPROPERTY()
    TObjectPtr<APawn> StoredPlayerPawn;

    /**
     * The current phase of battle
     */
    UPROPERTY()
    EBattlePhase Phase = EBattlePhase::Setup;

    /**
     * Delegate that is invoked when the battle ends
     */
    TPromise<EBattleResult> OnBattleEnd;

    int32 RunAttempts = 0;

    UPROPERTY(BlueprintGetter = GetBattleHUD, Category = "Battle|Display")
    TScriptInterface<IBattleHUD> BattleHUD;
};
