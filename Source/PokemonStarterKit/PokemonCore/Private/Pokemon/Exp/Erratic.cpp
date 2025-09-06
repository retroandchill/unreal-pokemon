// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Erratic.h"
#include "RetroLib/Utils/Math.h"

int32 Exp::FErratic::ExpForLevel(int32 Level) const
{
    check(Level > 0)

    if (Level == 1)
        return 0;

    if (Level <= 50)
        return Retro::Pow(Level, 3) * (100 - Level) / 50;

    if (Level <= 68)
        return Retro::Pow(Level, 3) * (150 - Level) / 100;

    if (Level <= 98)
        return Retro::Pow(Level, 3) * ((1911 - 10 * Level) / 3) / 500;

    return Retro::Pow(Level, 3) * (160 - Level) / 100;
}
