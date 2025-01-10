﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle.h"
#include "Battle/Actions/BattleAction.h"
#include "Effects/TurnBasedGameplayEffect.h"
#include "RetroLib/Ranges/Views/AnyView.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "UObject/Object.h"

#include "PokemonBattle.generated.h"

class ABattleSequencer;
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
    TScriptInterface<IBattle> Initialize(const FBattleInfo &BattleInfo) override;

  protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool IsTrainerBattle_Implementation() const override;
    void JumpToBattleScene_Implementation(APlayerController *PlayerController) override;

    void Tick(float DeltaSeconds) override;

  public:
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void StartBattle() override;

    void OnBattlersEnteringBattle(Retro::Ranges::TAnyView<TScriptInterface<IBattler>> Battlers);

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

    UFUNCTION(BlueprintPure, Category = "Battle|Content")
    const TScriptInterface<IBattleSide> &GetPlayerSide() const override;

    UFUNCTION(BlueprintPure, Category = "Battle|Content")
    const TScriptInterface<IBattleSide> &GetOpposingSide() const override;

    Retro::TGenerator<TScriptInterface<IBattleSide>> GetSides() const override;
    Retro::TGenerator<TScriptInterface<IBattler>> GetActiveBattlers() const override;
    void ExecuteAction(IBattleAction &Action) override;

  protected:
    bool RunCheck_Implementation(const TScriptInterface<IBattler> &Battler, bool bDuringBattle) override;
    void EndBattle_Implementation(EBattleResult Result) override;

  public:
    void BindToOnBattleEnd(FOnBattleEnd::FDelegate &&Callback) override;
    void ClearOnBattleEnd() override;

  protected:
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
    void QueueBattleIntro();

    /**
     * Display the intro message for the battle.
     */
    UFUNCTION(BlueprintPure, Category = "Battle|Flow")
    FText GetBattleIntroMessage() const;

    /**
     * Send out the opposing side Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void QueueOpponentSendOut();

    /**
     * Display the message for a Pokémon send out prior to the animation
     * @param Message The message to display
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void QueueOpponentSendOutMessage(const FText &Message);

    /**
     * Play the animation to send out the player's Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void QueuePlayerSendOut();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void QueuePlayerSendOutMessage(const FText &Message);

    /**
     * Create the HUD used for the battle system
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void CreateBattleHUD();

    /**
     * Refresh the battle HUD so that it is up to date
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void RefreshBattleHUD();

    /**
     * Display an action usage message with the given text
     * @param MessageText The text of the message to display
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void QueueDisplayAction(const FText &MessageText);

    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void ExecuteAction();

    /**
     * Process the result of the battle and exit
     * @param Result The outcome of the battle in question
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Flow")
    void QueueBattleResultAnimation(EBattleResult Result);

    /**
     * Exit the battle scene and return to the map
     * @param Result the result of the battle in question
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void ExitBattleScene(EBattleResult Result) const;

  private:
    void ProcessTurnDurationTrigger(ETurnDurationTrigger Trigger);

    /**
     * Run at the head of every turn. Increments the turn count and initiates action selection.
     */
    void StartTurn();

  protected:
    /**
     * Run all checks that need to be handled at the end of the turn
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void EndTurn();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Flow")
    void ClearActionSelection();

  private:
    /**
     * Process all of the actions about to be performed
     */
    void BeginActionProcessing();

    /**
     * Proceed to the next action in the queue
     */
    void NextAction();

    /**
     * Decide the battle for the side with the given index
     * @param SideIndex The index for the battle side that cannot continue
     */
    void DecideBattle(int32 SideIndex);

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

    /**
     * The list of sides in battle
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattleSide>> Sides;

    UPROPERTY()
    TObjectPtr<ABattleSequencer> BattleSequencer;

    /**
     * The class used to constructing the sides of the battle.
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes")
    TSoftClassPtr<ABattleSequencer> BattleSequencerClass;

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
    FOnBattleEnd OnBattleEnd;

    int32 RunAttempts = 0;
};
