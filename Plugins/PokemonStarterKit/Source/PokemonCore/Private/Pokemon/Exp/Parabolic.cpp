// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Parabolic.h"
#include "RetroLib/Utils/Math.h"

using namespace Exp;

int32 FParabolic::ExpForLevel(int32 Level) const {
    check(Level > 0)

    if (Level == 1)
        return 0;

    return Retro::Pow(Level, 3) * 6 / 5 - 15 * Retro::Pow(Level, 2) + 100 * Level - 140;
}
