// "Unreal Pokémon" created by Retro & Chill.
#include "MathUtilities.h"

int32 UMathUtilities::Pow(int32 Base, int32 Exponent) {
	return FMath::FloorToInt(FMath::Pow(static_cast<float>(Base), Exponent));
}

FVector UMathUtilities::Midpoint(const FVector& Start, const FVector& End) {
	return (Start + End) / 2;
}
