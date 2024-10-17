// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/BlueprintExceptionInfo.h"

namespace UE::Ranges {

    class UNREALRANGES_API FBlueprintException : public std::exception {
    public:
        template <typename... T>
            requires std::constructible_from<FBlueprintExceptionInfo, T...>
        explicit FBlueprintException(T&&... Args) : ExceptionInfo(Forward<T>(Args)...) {}
        
        [[nodiscard]] const char* what() const override;
        
        const FBlueprintExceptionInfo& GetExceptionInfo() const {
            return ExceptionInfo;
        }

    private:
        FBlueprintExceptionInfo ExceptionInfo;
    };
}