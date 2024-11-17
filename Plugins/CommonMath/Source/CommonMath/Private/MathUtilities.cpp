// "Unreal Pokémon" created by Retro & Chill.
#include "MathUtilities.h"

int32 UMathUtilities::Pow(int32 Base, int32 Exponent) {
    return FMath::FloorToInt(FMath::Pow(static_cast<float>(Base), Exponent));
}

FVector UMathUtilities::Midpoint(const FVector &Start, const FVector &End) {
    return (Start + End) / 2;
}

int32 UMathUtilities::RandomIntInRange(const FInt32Range &Range) {
    check(Range.HasLowerBound() && Range.HasUpperBound())
    return FMath::RandRange(Range.GetLowerBoundValue(), Range.GetUpperBoundValue());
}