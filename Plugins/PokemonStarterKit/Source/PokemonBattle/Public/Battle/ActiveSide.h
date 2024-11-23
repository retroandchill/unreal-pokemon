// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleSide.h"
#include "GameFramework/Actor.h"

#include "ActiveSide.generated.h"

class UTurnBasedEffectComponent;
class UBattleSideAbilitySystemComponent;
class IBattle;

USTRUCT()
struct POKEMONBATTLE_API FTrainerParty {
    GENERATED_BODY()

    UPROPERTY()
    TArray<TScriptInterface<IBattler>> Battlers;
};

/**
 * An active side of the combat system in battle
 */
UCLASS(Abstract)
class POKEMONBATTLE_API AActiveSide : public AActor, public IBattleSide {
    GENERATED_BODY()

  public:
    AActiveSide();

    TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TArray<TScriptInterface<IPokemon>> &Pokemon,
                                             bool ShowBackSprites) override;
    TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount,
                                             bool ShowBackSprites) override;

  protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  public:
    const FGuid &GetInternalId() const override;
    const TScriptInterface<IBattle> &GetOwningBattle() const override;

    UFUNCTION(BlueprintPure, Category = GameplayAbilities)
    UTurnBasedEffectComponent *GetTurnBasedEffectComponent() const override;

    UE::Ranges::TAnyView<UTurnBasedEffectComponent *> GetChildEffectComponents() const override;

    uint8 GetSideSize() const override;
    const FText &GetIntroText() const override;
    const TOptional<FText> &GetSendOutText() const override;

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    bool ShowBackSprites() const override;

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    void SendOutBattlers() const override;

    UFUNCTION(BlueprintPure, Category = "Battle|Battlers")
    const TArray<TScriptInterface<IBattler>> &GetBattlers() const override;

    UFUNCTION(BlueprintPure, Category = "Battle|Battlers")
    const TArray<TScriptInterface<ITrainer>> &GetTrainers() const override;

    const TArray<TScriptInterface<IBattler>> &GetTrainerParty(const TScriptInterface<ITrainer> &Trainer) const override;
    void SwapBattlerPositions(const TScriptInterface<ITrainer> &Trainer, int32 IndexA, int32 IndexB) override;

    bool CanBattle() const override;

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    FTransform GetBattlerSpawnPosition(uint8 Index);

  private:
    /**
     * The ability system component used for this actor
     */
    UPROPERTY()
    TObjectPtr<UBattleSideAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UTurnBasedEffectComponent> TurnBasedEffectComponent;

    FGuid InternalId;

    /**
     * The class used to create the actual battler actors
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Classes", meta = (MustImplement = "Battler"))
    TSoftClassPtr<AActor> BattlerClass;

    /**
     * The battle that owns this one
     */
    UPROPERTY()
    TScriptInterface<IBattle> OwningBattle;

    uint8 SideSize = 1;

    bool bShowBackSprites;

    /**
     * The battlers that are owned and controlled by this side
     */
    UPROPERTY()
    TArray<TScriptInterface<IBattler>> Battlers;

    /**
     * The list of trainers (if any exist)
     */
    UPROPERTY()
    TArray<TScriptInterface<ITrainer>> Trainers;

    UPROPERTY()
    TMap<FGuid, FTrainerParty> TrainerParties;

    FText IntroMessageText;

    UPROPERTY(EditAnywhere, Category = Battle)
    FText WildBattleTextFormat = NSLOCTEXT("PokemonBattles", "WildBattleTextFormat", "A wild {Pkmn} appeared!");

    UPROPERTY(EditAnywhere, Category = Battle)
    FText TrainerBattleTextFormat =
        NSLOCTEXT("PokemonBattles", "TrainerBattleTextFormat", "You are challenged by {Names}!");

    TOptional<FText> SendOutText;

    UPROPERTY(EditAnywhere, Category = Battle)
    FText PlayerSendOutTextFormat = NSLOCTEXT("PokemonBattles", "PlayerSendOutTextFormat", "Go  {Pkmn}!");

    UPROPERTY(EditAnywhere, Category = Battle)
    FText OpponentSendOutTextFormat =
        NSLOCTEXT("PokemonBattles", "OpponentSendOutTextFormat", "{Named} sent out {Pkmn}!");
};