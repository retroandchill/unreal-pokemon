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
    auto LowerBound = Range.GetLowerBound();
    auto UpperBound = Range.GetUpperBound();
    return FMath::RandRange(LowerBound.IsInclusive() ? LowerBound.GetValue() : LowerBound.GetValue() + 1,
                            UpperBound.IsInclusive() ? UpperBound.GetValue() : UpperBound.GetValue() - 1);
}