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
 * The actual battle class used for battle flow.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API APokemonBattle : public AActor, public IBattle {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Battle|Initiation")
    void CreateWildBattle(const FPokemonDTO& Pokemon);
    
    TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>>&& SidesIn) override;

protected:
    void JumpToBattleScene_Implementation(APlayerController* PlayerController) override;
    
public:
    void StartBattle() override;
    
    void QueueAction(TUniquePtr<IBattleAction>&& Action) override;
    bool ActionSelectionFinished() const override;

    bool ShouldIgnoreAbilities() const override;
    void ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const override;
    void ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const override;
    bool FindGlobalAbility(FName AbilityID) const override;

    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Battle|Visuals")
    APawn* GetBattlePawn() const final;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    FTransform GetPlayerSidePosition() const;

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
    void ProcessBattleIntroMessage(const FText& Message);

    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void OpponentSendOut();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessOpponentSendOutMessage(const FText& Message);

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void OpponentSendOutAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessOpponentSendOutAnimation(const TScriptInterface<IBattleSide> &OpponentSide);

    UFUNCTION(BlueprintCallable, Category = "Battle|Flow")
    void PlayerSendOut();

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void PlayerSendOutAnimation();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessPlayerSendOutMessage(const FText& Message);

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void ProcessPlayerSendOutAnimation(const TScriptInterface<IBattleSide> &PlayerSide);

private:
    void StartTurn();

    uint32 TurnCount = 0;
    
    /**
     * The settings for the battle used to determine a large number of things about combat.
     */
    UPROPERTY()
    FBattleSettings BattleSettings;

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
     * Collection used to serve as the queue for actions.
     */
    TArray<TUniquePtr<IBattleAction>> ActionQueue;

    TMap<FGuid, uint8> CurrentActionCount;
    
    TMap<FGuid, uint8> ExpectedActionCount;

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
    
};
