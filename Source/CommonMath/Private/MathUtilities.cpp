// "Unreal Pokémon" created by Retro & Chill.
#include "MathUtilities.h"

template <typename T>
T LinearInterpolation_Internal(T StartValue, T EndValue, T Duration, T Delta) {
	if (Duration <= 0)
		return EndValue;

	if (Delta <= 0)
		return StartValue;

	if (Delta >= Duration)
		return EndValue;

	return StartValue + (EndValue - StartValue) * Delta / Duration;
}

double UMathUtilities::LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta) {
	return LinearInterpolation_Internal(StartValue, EndValue, Duration, Delta);
}

float UMathUtilities::LinearInterpolationF(float StartValue, float EndValue, float Duration, float Delta) {
	return LinearInterpolation_Internal(StartValue, EndValue, Duration, Delta);
}

int32 UMathUtilities::Pow(int32 Base, int32 Exponent) {
	return FMath::FloorToInt(FMath::Pow(static_cast<float>(Base), Exponent));
}
