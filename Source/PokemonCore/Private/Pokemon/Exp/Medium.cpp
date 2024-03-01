// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Medium.h"

using namespace Exp;

int32 FMedium::ExpForLevel(int32 Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3);
}

TUniquePtr<IGrowthRate> FMedium::Clone() const {
	return MakeUnique<FMedium>(*this);
}
