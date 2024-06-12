// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleSide.h"
#include "GameFramework/Actor.h"
#include "ActiveSide.generated.h"

class IBattle;

/**
 * An active side of the combat system in battle
 */
UCLASS(Abstract)
class POKEMONBATTLE_API AActiveSide : public AActor, public IBattleSide {
    GENERATED_BODY()

public:
    TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TScriptInterface<IPokemon> &Pokemon, bool ShowBackSprites) override;
    TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount, bool ShowBackSprites) override;

    TScriptInterface<IBattle> GetOwningBattle() const override;
    uint8 GetSideSize() const override;

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    bool ShowBackSprites() const override;
    
    const TArray<TScriptInterface<IBattler>> & GetBattlers() const override;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    FTransform GetBattlerSpawnPosition(uint8 Index);

private:
    /**
     * The class used to create the actual battler actors
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes", meta = (MustImplement = "Battler"))
    TSoftClassPtr<AActor> BattlerClass;
    
    /**
     * The battle that owns this one
     */
    TWeakInterfacePtr<IBattle> OwningBattle;

    uint8 SideSize = 1;

    bool bShowBackSprites;

    /**
     * The battlers that are owned and controlled by this side
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattler>> Battlers;

};