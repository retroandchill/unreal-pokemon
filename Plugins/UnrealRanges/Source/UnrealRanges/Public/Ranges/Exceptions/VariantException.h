// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintException.h"
#include "Ranges/Variants/VariantObject.h"

namespace UE::Ranges {
    /**
     * Exception for when a {@link TVariantObject} is attempted to be used incorrectly.
     */
    class UNREALRANGES_API FVariantException : public FBlueprintException {
      public:
        /**
         * Construct a new exception with the given arguments
         * @tparam T The arguments to {@link FBlueprintException} that are used for construction
         * @param Args The arguments to {@link FBlueprintException} that are used for construction
         */
        template <typename... T>
            requires std::constructible_from<FBlueprintException, T...> &&
                     (sizeof...(T) != 1 || !(std::derived_from<std::remove_cvref_t<T>, FVariantException> || ...))
        explicit FVariantException(T &&...Args) : FBlueprintException(Forward<T>(Args)...) {
        }
    };
} // namespace UE::Ranges