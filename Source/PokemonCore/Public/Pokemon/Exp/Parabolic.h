// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Parabolic Exp. Growth Scheme
	 */
	class POKEMONCORE_API FParabolic : public IGrowthRate {
	public:
		int32 ExpForLevel(int32 Level) const override;
		virtual TUniquePtr<IGrowthRate> Clone() const override;
	};
}