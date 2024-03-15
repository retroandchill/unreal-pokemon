// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "GrowthRate.h"


namespace Exp {
	/**
	 * Medium/Medium Fast Exp Growth curve
	 */
	class POKEMONCORE_API FMedium : public IGrowthRate {
	public:
		~FMedium() override = default;
		int32 ExpForLevel(int32 Level) const override;
		TUniquePtr<IGrowthRate> Clone() const override;
	};
}
