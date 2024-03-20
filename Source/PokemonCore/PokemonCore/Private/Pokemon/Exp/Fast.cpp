// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Exp/Fast.h"

#include "Asserts.h"
#include "MathUtilities.h"

using namespace Exp;

int32 FFast::ExpForLevel(int32 Level) const {
	ASSERT(Level > 0)

	if (Level == 1)
		return 0;

	return UMathUtilities::Pow(Level, 3) * 4 / 5;
}

TUniquePtr<IGrowthRate> FFast::Clone() const {
	return MakeUnique<FFast>(*this);
}
