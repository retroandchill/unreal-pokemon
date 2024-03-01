// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Fast Exp. Growth Scheme
	 */
	class POKEMONCORE_API FFast : public IGrowthRate {

	public:
		int32 ExpForLevel(int32 Level) const override;
		TUniquePtr<IGrowthRate> Clone() const override;
	};
}