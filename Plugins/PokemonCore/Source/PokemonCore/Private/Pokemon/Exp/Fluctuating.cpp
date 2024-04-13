// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Fluctuating.h"

#include "MathUtilities.h"

using namespace Exp;

int32 FFluctuating::ExpForLevel(int32 Level) const {
    check(Level > 0)

    if (Level == 1)
        return 0;

    if (Level <= 15)
        return UMathUtilities::Pow(Level, 3) * (24 + (Level + 1) / 3) / 50;

    if (Level <= 35)
        return UMathUtilities::Pow(Level, 3) * (14 + Level) / 50;

    return UMathUtilities::Pow(Level, 3) * (32 + Level / 2) / 50;
}
