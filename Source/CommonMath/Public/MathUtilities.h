// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathUtilities.generated.h"

/**
 * Common utility functions for math
 */
UCLASS()
class COMMONMATH_API UMathUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Linear interpolation between two values, given the duration of the change and the time passed since the start of
	 * the change (delta)
	 * @param StartValue The starting value for the change
	 * @param EndValue Where the value should be at the end of the duration
	 * @param Duration The duration of the change
	 * @param Delta The change in time
	 * @return The interpolated value
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Math")
	static double LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta);

	/**
	 * Linear interpolation between two values, given the duration of the change and the time passed since the start of
	 * the change (delta)
	 * @param StartValue The starting value for the change
	 * @param EndValue Where the value should be at the end of the duration
	 * @param Duration The duration of the change
	 * @param Delta The change in time
	 * @return The interpolated value
	 */
	static float LinearInterpolationF(float StartValue, float EndValue, float Duration, float Delta);
	
};
