// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/BattleSide.h"
#include <gmock/gmock-function-mocker.h>

/**
 * 
 */
class POKEMONTESTS_API FMockBattleSide : public IBattleSide {
public:
    ~FMockBattleSide() override = default;

    MOCK_METHOD(TScriptInterface<IBattleSide>, Initialize, (const TScriptInterface<IBattle> &Battle, const TScriptInterface<IPokemon> &Pokemon, bool ShowBackSprites), (override));
    MOCK_METHOD(TScriptInterface<IBattleSide>, Initialize, (const TScriptInterface<IBattle> &Battle, const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount, bool ShowBackSprites), (override));
    MOCK_METHOD(const FGuid &, GetInternalId, (), (const override));
    MOCK_METHOD(const TScriptInterface<IBattle> &, GetOwningBattle, (), (const override));
    MOCK_METHOD(uint8, GetSideSize, (), (const override));
    MOCK_METHOD(const FText &, GetIntroText, (), (const override));
    MOCK_METHOD(const TOptional<FText> &, GetSendOutText, (), (const override));
    MOCK_METHOD(bool, ShowBackSprites, (), (const override));
    MOCK_METHOD(void, SendOutBattlers, (), (const override));
    MOCK_METHOD(const TArray<TScriptInterface<IBattler>> &, GetBattlers, (), (const override));
    MOCK_METHOD(bool, CanBattle, (), (const override));
};
