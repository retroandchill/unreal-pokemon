// "Unreal Pokémon" created by Retro & Chill.
#include "DataTypes/OptionalUtilities.h"

POKEMONUTILITIES_API bool OptionalUtilities::OptionalsSame(const TOptional<FText> &A, const TOptional<FText> &B) {
    if (A.IsSet() != B.IsSet())
        return false;

    return A.IsSet() ? A.GetValue().EqualTo(B.GetValue()) : true;
}