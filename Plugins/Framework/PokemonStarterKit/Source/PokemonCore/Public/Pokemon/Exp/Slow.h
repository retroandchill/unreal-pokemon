// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"

namespace Exp {
    /**
     * Represents the Slow Exp. Growth Scheme
     */
    class POKEMONCORE_API FSlow : public IGrowthRate {
      public:
        ~FSlow() override = default;
        int32 ExpForLevel(int32 Level) const override;
    };
} // namespace Exp
