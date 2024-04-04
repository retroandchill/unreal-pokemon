// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Erratic Exp. Growth Scheme
	 */
	class POKEMONCORE_API FErratic : public IGrowthRate {
	public:
		~FErratic() override = default;
		int32 ExpForLevel(int32 Level) const override;
	};
}
