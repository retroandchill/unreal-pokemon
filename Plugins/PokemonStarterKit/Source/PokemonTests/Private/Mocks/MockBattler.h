// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battlers/Battler.h"
#include <gmock/gmock.h>

class FMockBattler : public IBattler {
public:
    ~FMockBattler() override = default;

    MOCK_METHOD3(Initialize, TScriptInterface<IBattler>(const TScriptInterface<IBattleSide> &Side, const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately));
    MOCK_METHOD(FGuid, GetInternalId, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattleSide> &, GetOwningSide, (), (const, override));
    MOCK_METHOD(FText, GetNickname, (), (const, override));
    MOCK_METHOD(EPokemonGender, GetGender, (), (const, override));
    MOCK_METHOD(int32, GetPokemonLevel, (), (const, override));
    MOCK_METHOD(int32, GetHP, (), (const, override));
    MOCK_METHOD(int32, GetMaxHP, (), (const, override));
    MOCK_METHOD(float, GetHPPercent, (), (const, override));
    MOCK_METHOD(void, TakeBattleDamage, (int32 Damage), (override));
    MOCK_METHOD(bool, IsFainted, (), (const, override));
    MOCK_METHOD(void, Faint, (), (const, override));
    MOCK_METHOD(int32, GetAttack, (), (const, override));
    MOCK_METHOD(int32, GetDefense, (), (const, override));
    MOCK_METHOD(int32, GetSpecialAttack, (), (const, override));
    MOCK_METHOD(int32, GetSpecialDefense, (), (const, override));
    MOCK_METHOD(int32, GetSpeed, (), (const, override));
    MOCK_METHOD(float, GetExpPercent, (), (const, override));
    MOCK_METHOD(TArray<FName>, GetTypes, (), (const, override));
    MOCK_METHOD(bool, IsAbilityActive, (), (const, override));
    MOCK_METHOD(UAbilityBattleEffect *, GetAbility, (), (const, override));
    MOCK_METHOD(bool, IsHoldItemActive, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IHoldItemBattleEffect> &, GetHoldItem, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattleMove>> &, GetMoves, (), (const, override));
    MOCK_METHOD(void, SelectActions, (), (override));
    MOCK_METHOD(uint8, GetActionCount, (), (const, override));
    MOCK_METHOD(ranges::any_view<TScriptInterface<IBattler>>, GetAllies, (), (const, override));
    MOCK_METHOD(ranges::any_view<IIndividualTraitHolder *>, GetTraitHolders, (), (const, override));
    MOCK_METHOD(void, ShowSprite, (), (const, override));
};