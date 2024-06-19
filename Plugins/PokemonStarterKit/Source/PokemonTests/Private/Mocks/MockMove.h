// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMove.h"
#include <gmock/gmock.h>

class FMockBattleMove : public IBattleMove {
public:
    ~FMockBattleMove() override = default;

    // TODO: Mock the rest of the moves
};