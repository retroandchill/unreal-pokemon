// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp
{
    /**
     * Represents the Parabolic Exp. Growth Scheme
     */
    class POKEMONCORE_API FParabolic : public IGrowthRate
    {
      public:
        ~FParabolic() override = default;
        int32 ExpForLevel(int32 Level) const override;
    };
} // namespace Exp
