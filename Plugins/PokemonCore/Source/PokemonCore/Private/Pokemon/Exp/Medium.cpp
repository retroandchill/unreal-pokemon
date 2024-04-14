// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Medium.h"
#include "MathUtilities.h"

using namespace Exp;

int32 FMedium::ExpForLevel(int32 Level) const {
    check(Level > 0)

    if (Level == 1)
        return 0;

    return UMathUtilities::Pow(Level, 3);
}
