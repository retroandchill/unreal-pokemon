// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintException.h"

namespace UE::Ranges {
    /**
     * Exception for where there is an invalid argument in a Blueprint function.
     */
    class UNREALRANGES_API FInvalidArgumentException : public FBlueprintException {
    public:
        /**
         * Construct a new exception with the given arguments
         * @tparam T The arguments to {@link FBlueprintException} that are used for construction
         * @param Args The arguments to {@link FBlueprintException} that are used for construction
         */
        template <typename... T>
            requires std::constructible_from<FBlueprintException, T...> && (sizeof...(T) != 1 ||
                !(std::derived_from<std::remove_cvref_t<T>, FInvalidArgumentException> || ...))
        explicit FInvalidArgumentException(T&&... Args) : FBlueprintException(Forward<T>(Args)...) {}
    };
}