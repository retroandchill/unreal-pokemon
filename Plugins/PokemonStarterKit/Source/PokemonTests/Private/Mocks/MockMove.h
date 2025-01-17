﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Mocking/UnrealMock.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include "Pokemon/Moves/Move.h"

THIRD_PARTY_INCLUDES_START
#include <gmock/gmock.h>
THIRD_PARTY_INCLUDES_END

class FMockMove : public IMove {
    DECLARE_MOCK_INTERFACE(FMockMove)

  public:
    MOCK_METHOD(const FMoveData &, GetMoveData, (), (const, override));
    MOCK_METHOD(FName, GetType, (), (const, override));
    MOCK_METHOD(EMoveDamageCategory, GetDamageCategory, (), (const, override));
    MOCK_METHOD(int32, GetBasePower, (), (const, override));
    MOCK_METHOD(int32, GetAccuracy, (), (const, override));
    MOCK_METHOD(int32, GetCurrentPP, (), (const, override));
    MOCK_METHOD(int32, GetTotalPP, (), (const, override));
    MOCK_METHOD(FName, GetFunctionCode, (), (const, override));
    MOCK_METHOD(const FMoveTarget &, GetTargetType, (), (const, override));
    MOCK_METHOD(const TArray<FName> &, GetTags, (), (const, override));
    MOCK_METHOD(void, DecrementPP, (int32 Amount), (override));
};