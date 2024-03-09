// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Erratic.h"

#include "MathUtilities.h"

using namespace Exp;

int32 FErratic::ExpForLevel(int32 Level) const {
	check(Level > 0)

	if (Level == 1)
		return 0;
	
	if (Level <= 50)
		return UMathUtilities::Pow(Level, 3) * (100 - Level) / 50;

	if (Level <= 68)
		return UMathUtilities::Pow(Level, 3) * (150 - Level) / 100;

	if (Level <= 98)
		return UMathUtilities::Pow(Level, 3) * ((1911 - 10 * Level) / 3) / 500;

	return UMathUtilities::Pow(Level, 3) * (160 - Level) / 100;
}

TUniquePtr<IGrowthRate> FErratic::Clone() const {
	return MakeUnique<FErratic>(*this);
}
