// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistry.h"

namespace Exp {
	/**
	 * Interface to represent various Exp. grow rates
	 */
	class POKEMONDATA_API GrowthRate {
		DECLARE_REGISTRY_CLASS(GrowthRate)

	public:
		virtual ~GrowthRate();

		/**
		 * Gets the amount of Exp that is required to reach a certain level 
		 * @param Level The level in to calculate the Exp. for
		 * @return The amount of Exp. required to level up
		 * @throws std::invalid_argument If the level passed in in either 0 or negative
		 */
		virtual int ExpForLevel(int Level) const = 0;
	};
}
