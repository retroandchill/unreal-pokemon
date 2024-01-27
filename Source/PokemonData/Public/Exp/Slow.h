// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	 * 
	 */
	class POKEMONDATA_API Slow : public GrowthRate
	{
	public:
		virtual int ExpForLevel(int Level) const override;
	};
}
