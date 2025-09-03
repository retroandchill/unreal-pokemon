// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
    /**
     * Represents the Fluctuating Exp. Growth Scheme
     */
    class POKEMONCORE_API FFluctuating : public IGrowthRate {
      public:
        ~FFluctuating() override = default;
        int32 ExpForLevel(int32 Level) const override;
    };
} // namespace Exp
