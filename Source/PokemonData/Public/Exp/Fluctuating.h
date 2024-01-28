// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	* Represents the Fluctuating Exp. Growth Scheme
	*/
	class POKEMONDATA_API FFluctuating : public IGrowthRate {
	public:
		virtual int ExpForLevel(int Level) const override;
	};
}
