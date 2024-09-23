// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Status.h"
#include "Species/SpeciesData.h"
#include <gmock/gmock.h>

class FMockBattler : public IBattler {
  public:
    ~FMockBattler() override = default;

    MOCK_METHOD3(Initialize,
                 TScriptInterface<IBattler>(const TScriptInterface<IBattleSide> &Side,
                                            const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately));
    MOCK_METHOD(FGuid, GetInternalId, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetOwningSide, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IPokemon> &, GetWrappedPokemon, (), (const, override));
    MOCK_METHOD(bool, IsActive, (), (const, override));
    MOCK_METHOD(const FSpeciesData &, GetSpecies, (), (const, override));
    MOCK_METHOD(FText, GetNickname, (), (const, override));
    MOCK_METHOD(EPokemonGender, GetGender, (), (const, override));
    MOCK_METHOD(int32, GetPokemonLevel, (), (const, override));
    MOCK_METHOD(void, RefreshStats, (), (override));
    MOCK_METHOD(float, GetHPPercent, (), (const, override));
    MOCK_METHOD(void, TakeBattleDamage, (int32 Damage), (override));
    MOCK_METHOD(bool, IsFainted, (), (const, override));
    MOCK_METHOD(bool, IsNotFainted, (), (const, override));
    MOCK_METHOD(void, Faint, (), (const, override));
    MOCK_METHOD(bool, CanGainExp, (), (const, override));
    MOCK_METHOD(float, GetExpPercent, (), (const, override));
    MOCK_METHOD(TArray<FExpGainInfo>, GiveExpToParticipants, (), (override));
    MOCK_METHOD2(GainExpAndEVs, FLevelUpStatChanges(int32 Exp, const TMap<FName, uint8> &EVs));
    MOCK_METHOD(TArray<FName>, GetTypes, (), (const, override));
    MOCK_METHOD(UBattlerAbilityComponent *, GetAbilityComponent, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattleMove>> &, GetMoves, (), (const, override));
    MOCK_METHOD(FText, GetRecallMessage, (), (const, override));
    MOCK_METHOD(FGameplayAbilitySpecHandle, PerformSwitch, (const TScriptInterface<IBattler> &SwitchTarget),
                (override));
    MOCK_METHOD(bool, IsOwnedByPlayer, (), (const, override));
    MOCK_METHOD(void, SelectActions, (), (override));
    MOCK_METHOD(void, RequireSwitch, (), (override));
    MOCK_METHOD(uint8, GetActionCount, (), (const, override));
    MOCK_METHOD(int32, GetTurnCount, (), (const, override));
    MOCK_METHOD(UE::Ranges::TAnyView<TScriptInterface<IBattler>>, GetAllies, (), (const, override));
    MOCK_METHOD(void, ShowSprite, (const FVector &Offset), (const, override));
    MOCK_METHOD(void, HideSprite, (), (const, override));
    MOCK_METHOD(void, RecordParticipation, (), (override));
    MOCK_METHOD(void, AddParticipant, (const TScriptInterface<IBattler> &Participant), (override));
    MOCK_METHOD(const TOptional<FStatusEffectInfo> &, GetStatusEffect, (), (const, override));
    MOCK_METHOD2(InflictStatusEffect, void(FStatusHandle StatusEffectID, FActiveGameplayEffectHandle EffectHandle));
    MOCK_METHOD(void, CureStatusEffect, (), (override));
};