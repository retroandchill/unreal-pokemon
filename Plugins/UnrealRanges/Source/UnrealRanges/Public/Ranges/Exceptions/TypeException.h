// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintException.h"

namespace UE::Ranges {
    /**
     * Exception for where there is a type mismatch in a Blueprint function.
     */
    class UNREALRANGES_API FTypeException : public FBlueprintException {
    public:
        /**
         * Construct a new exception with the given arguments
         * @tparam T The arguments to {@link FBlueprintException} that are used for construction
         * @param Args The arguments to {@link FBlueprintException} that are used for construction
         */
        template <typename... T>
            requires std::constructible_from<FBlueprintException, T...> && (sizeof...(T) != 1 ||
                !(std::derived_from<std::remove_cvref_t<T>, FTypeException> || ...))
        explicit FTypeException(T&&... Args) : FBlueprintException(Forward<T>(Args)...) {}
    };
}