// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Parabolic Exp. Growth Scheme
	 */
	class POKEMONDATA_API FParabolic final : public IGrowthRate {
	public:
		virtual int ExpForLevel(int Level) const override;
	};
}
