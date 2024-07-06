// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/BattleSide.h"
#include <gmock/gmock.h>

/**
 * 
 */
class FMockBattleSide : public IBattleSide {
public:
    ~FMockBattleSide() override = default;

    MOCK_METHOD3(Initialize, TScriptInterface<IBattleSide>(const TScriptInterface<IBattle> &Battle, const TArray<TScriptInterface<IPokemon>> &Pokemon, bool ShowBackSprites));
    MOCK_METHOD4(Initialize, TScriptInterface<IBattleSide>(const TScriptInterface<IBattle> &Battle, const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount, bool ShowBackSprites));
    MOCK_METHOD(const FGuid &, GetInternalId, (), (const, override));
    MOCK_METHOD(const TScriptInterface<IBattle> &, GetOwningBattle, (), (const, override));
    MOCK_METHOD(uint8, GetSideSize, (), (const, override));
    MOCK_METHOD(const FText &, GetIntroText, (), (const, override));
    MOCK_METHOD(const TOptional<FText> &, GetSendOutText, (), (const, override));
    MOCK_METHOD(bool, ShowBackSprites, (), (const, override));
    MOCK_METHOD(void, SendOutBattlers, (), (const, override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattler>> &, GetBattlers, (), (const, override));
    MOCK_METHOD(bool, CanBattle, (), (const, override));
};
