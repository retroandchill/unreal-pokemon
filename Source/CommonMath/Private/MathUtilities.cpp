// "Unreal Pokémon" created by Retro & Chill.
#include "MathUtilities.h"

int32 UMathUtilities::Pow(int32 Base, int32 Exponent) {
	return FMath::FloorToInt(FMath::Pow(static_cast<float>(Base), Exponent));
}
