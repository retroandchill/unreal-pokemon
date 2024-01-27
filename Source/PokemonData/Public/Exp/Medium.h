// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrowthRate.h"

namespace Exp {
	/**
	 * Medium/Medium Fast Exp Growth curve
	 */
	class POKEMONDATA_API Medium final : public GrowthRate {

	public:
		virtual int ExpForLevel(int Level) const override;
	};
}