// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrowthRate.h"

namespace Exp {
	/**
	 * Represents the Erratic Exp. Growth Scheme
	 */
	class POKEMONDATA_API Erratic : public GrowthRate {

	public:
		virtual FName GetId() const override;
		virtual int ExpForLevel(int Level) const override;
	};
}
