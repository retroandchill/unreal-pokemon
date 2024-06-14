// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle.h"
#include "BattleSettings.h"
#include "Pokemon/PokemonDTO.h"
#include "UObject/Object.h"

#include "PokemonBattle.generated.h"

class IBattleAction;
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
    /**
     * Create a wild Pokémon battle with the given settings
     * @param Pokemon The Pokémon to generate from
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Initiation")
    void CreateWildBattle(const FPokemonDTO &Pokemon);

    TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) override;

  protected:
    void JumpToBattleScene_Implementation(APlayerController *PlayerController) override;

    void Tick(float DeltaSeconds) override;

  public:
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void StartBattle() override;

    void QueueAction(TUniquePtr<IBattleAction> &&Action) override;
    bool ActionSelectionFinished() const override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Battle|Visuals")
    APawn *GetBattlePawn() const final;

    bool ShouldIgnoreAbilities() const override;
    void ForEachSide(const TFunctionRef<void(int32, const TScriptInterface<IBattleSide> &)> &Callback) const override;
    void ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const override;
    void ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const override;
    bool FindGlobalAbility(FName AbilityID) const override;
    void ExecuteAction(IBattleAction &Action) override;

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
    void PlayBattleIntro();

    /**
     * Display the intro message for the battle.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void DisplayBattleIntroMessage();

    /**
     * The helper function used to display the intro message to the player
     * @param Message The message to display
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessBattleIntroMessage(const FText &Message);

    /**
     * Send out the opposing side Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void OpponentSendOut();

    /**
     * Display the message for a Pokémon send out prior to the animation
     * @param Message The message to display
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessOpponentSendOutMessage(const FText &Message);

    /**
     * Play the animation to send out an opponent's Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void OpponentSendOutAnimation();

    /**
     * Actually play the animation in question with the given side information
     * @param OpponentSide The actual side structure for the opponents
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessOpponentSendOutAnimation(const TScriptInterface<IBattleSide> &OpponentSide);

    /**
     * Play the animation to send out the player's Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void PlayerSendOut();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessPlayerSendOutMessage(const FText &Message);

    /**
     * Play the actual animation to
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void PlayerSendOutAnimation();

    /**
     * Actually play the animation in question with the given side information
     * @param PlayerSide The actual side structure for the player
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessPlayerSendOutAnimation(const TScriptInterface<IBattleSide> &PlayerSide);

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
    void DisplayAction(const FText &MessageText);

    /**
     * Apply the result of the current action
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void ApplyActionResult();

  private:
    /**
     * Run at the head of every turn. Increments the turn count and initiates action selection.
     */
    void StartTurn();

    /**
     * Process all of the actions about to be performed
     */
    void BeginActionProcessing();
    
    /**
    * Proceed to the next action in the queue
    */
    void NextAction();

    /**
     * The current turn number that we're on in battle.
     */
    uint32 TurnCount = 0;

    /**
     * The settings for the battle used to determine a large number of things about combat.
     */
    UPROPERTY()
    FBattleSettings BattleSettings;

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

    /**
     * The actual queue of actions to be executed
     */
    TQueue<TUniquePtr<IBattleAction>> ActionQueue;

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
    EBattlePhase Phase = EBattlePhase::Setup;

    /**
     * Have the action messages for the current action been displayed
     */
    bool bActionMessagesDisplayed = false;
};
