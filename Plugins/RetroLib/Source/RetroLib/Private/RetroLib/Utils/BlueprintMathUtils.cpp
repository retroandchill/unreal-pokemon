// "Unreal Pokémon" created by Retro & Chill.
#include "RetroLib/Utils/BlueprintMathUtils.h"

FVector UBlueprintMathUtils::Midpoint(const FVector &Start, const FVector &End)
{
    return (Start + End) / 2;
}

int32 UBlueprintMathUtils::RandomIntInRange(const FInt32Range &Range)
{
    check(Range.HasLowerBound() && Range.HasUpperBound())
    auto LowerBound = Range.GetLowerBound();
    auto UpperBound = Range.GetUpperBound();
    return FMath::RandRange(LowerBound.IsInclusive() ? LowerBound.GetValue() : LowerBound.GetValue() + 1,
                            UpperBound.IsInclusive() ? UpperBound.GetValue() : UpperBound.GetValue() - 1);
}