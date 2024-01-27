// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistry.h"

namespace Exp {
	/**
	 * Interface to represent various Exp. grow rates
	 */
	class POKEMONDATA_API GrowthRate {
		DECLARE_REGISTRY_CLASS(GrowthRate, FName)
		
	public:
		virtual ~GrowthRate();
		
		/**
		 * Gets the ID of the growth rate used to retrieve it from the table
		 * @return The ID of the growth rate in question
		 */
		virtual FName GetId() const = 0;

		/**
		 * Gets the amount of Exp that is required to reach a certain level 
		 * @param Level The level in to calculate the Exp. for
		 * @return The amount of Exp. required to level up
		 */
		virtual int ExpForLevel(int Level) const = 0;
	};
}
