// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle.h"
#include "BattleSettings.h"
#include "UObject/Object.h"
#include "PokemonBattle.generated.h"

class IBattleAction;
class IBattleSide;
class ITrainer;

/**
 * The actual battle class used for battle flow.
 */
UCLASS()
class POKEMONBATTLE_API UPokemonBattle : public UObject, public IBattle {
    GENERATED_BODY()

public:
    TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>>&& SidesIn) override;
    void StartBattle() override;
    void QueueAction(TUniquePtr<IBattleAction>&& Action) override;
    bool ActionSelectionFinished() const override;

    bool ShouldIgnoreAbilities() const override;
    void ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const override;
    void ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect> &)> Callback) const override;
    bool FindGlobalAbility(FName AbilityID) const override;

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
     * Mutex object used to lock the action queue while it is being edited.
     */
    FCriticalSection ActionMutex;

    /**
     * Collection used to serve as the queue for actions.
     */
    TArray<TUniquePtr<IBattleAction>> ActionQueue;

    TMap<FGuid, uint8> CurrentActionCount;
    
    TMap<FGuid, uint8> ExpectedActionCount;
    
};
