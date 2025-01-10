// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Fast.h"
#include "RetroLib/Utils/Math.h"


int32 Exp::FFast::ExpForLevel(int32 Level) const {
    check(Level > 0)

    if (Level == 1)
        return 0;

    return Retro::Pow(Level, 3) * 4 / 5;
}
