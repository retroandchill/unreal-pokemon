// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Fast Exp. Growth Scheme
	 */
	class POKEMONDATA_API Fast : public GrowthRate
	{
	public:
		virtual int ExpForLevel(int Level) const override;
	};
}
	
