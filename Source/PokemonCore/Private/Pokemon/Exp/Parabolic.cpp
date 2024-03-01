// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Parabolic.h"

using namespace Exp;

int32 FParabolic::ExpForLevel(int32 Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 6 / 5 - 15 * std::pow(Level, 2) + 100 * Level - 140;
}

TUniquePtr<IGrowthRate> FParabolic::Clone() const {
	return MakeUnique<FParabolic>(*this);
}
