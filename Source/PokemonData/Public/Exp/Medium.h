// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrowthRate.h"

namespace Exp {
	/**
	 * Medium/Medium Fast Exp Growth curve
	 */
	class POKEMONDATA_API FMedium final : public IGrowthRate {
	public:
		virtual int ExpForLevel(int Level) const override;
	};
}
