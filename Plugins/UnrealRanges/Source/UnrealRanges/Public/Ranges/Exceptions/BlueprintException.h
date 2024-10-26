// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/BlueprintExceptionInfo.h"

#include <string>

namespace UE::Ranges {

    /**
     * Represents a basic Blueprint exception that is raised by the game.
     */
    class UNREALRANGES_API FBlueprintException : public std::exception {
    public:
        /**
         * Construct a new exception with the given arguments
         * @tparam T The arguments to {@link FBlueprintExceptionInfo} that are used for construction
         * @param Args The arguments to {@link FBlueprintExceptionInfo} that are used for construction
         */
        template <typename... T>
        requires std::constructible_from<FBlueprintExceptionInfo, T...> && (sizeof...(T) != 1 ||
            !(std::same_as<std::remove_cvref_t<T>, FBlueprintException> || ...))
        explicit FBlueprintException(T&&... Args) : ExceptionInfo(Forward<T>(Args)...),
            NativeMessage(TCHAR_TO_ANSI(*ExceptionInfo.GetDescription().ToString())) {}
        
        [[nodiscard]] const char* what() const override;

        /**
         * Get the underlying info about the exception
         * @return The underlying info about the exception
         */
        const FBlueprintExceptionInfo& GetExceptionInfo() const {
            return ExceptionInfo;
        }

    private:
        FBlueprintExceptionInfo ExceptionInfo;
        std::string NativeMessage;
    };
}