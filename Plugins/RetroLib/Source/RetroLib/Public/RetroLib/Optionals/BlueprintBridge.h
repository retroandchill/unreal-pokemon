// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "BlueprintBridge.generated.h"
#include "RetroLib/Concepts/Interfaces.h"
#include "RetroLib/TypeTraits.h"
#include "UObject/Object.h"

/**
 * @enum ESetStatus
 * @brief Enumerates the status of an optional operation.
 *
 * Represents whether a value is set or not. This enum can be utilized in various logic
 * to indicate the presence or absence of a value.
 */
UENUM(BlueprintType)
enum class ESetStatus : uint8 {
    /**
     * The value is set.
     */
    IsSet,

    /**
     * The value is not set.
     */
    NotSet
};

namespace Retro::Optionals {
    namespace Detail {
        template <typename>
        struct TBlueprintOptionalGetter;

        template <typename T>
            requires(!std::is_reference_v<T>)
        struct TBlueprintOptionalGetter<TOptional<T>> {
            using ValueType = std::remove_const_t<T>;
        };

        template <typename T>
        struct TBlueprintOptionalGetter<TOptional<T &>> {
            using ValueType = std::remove_const_t<T>;
        };

        template <typename T>
            requires std::derived_from<T, UObject>
        struct TBlueprintOptionalGetter<TOptional<T &>> {
            using ValueType = std::remove_const_t<T> *;
        };

        template <typename T>
            requires UnrealInterface<T>
        struct TBlueprintOptionalGetter<TOptional<T &>> {
            using ValueType = TScriptInterface<std::remove_const_t<T>>;
        };
    } // namespace Detail

    /**
     * Get the type of the out parameter for a Blueprint utility function for getting the value of an optional property.
     *
     * @tparam T The type of the optional
     */
    template <typename T>
    using TBlueprintOutParameter = typename Detail::TBlueprintOptionalGetter<T>::ValueType;

    /**
     * Is this an optional type that can be gotten out as a Blueprint parameter?
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept BlueprintCompatibleOptional =
        SpecializationOf<T, TOptional> && requires { typename TBlueprintOutParameter<T>; };

    /**
     * Retrieves the value from an optional and sets it to the output parameter.
     *
     * @tparam T The type of the optional parameter.
     * @param Optional The optional parameter whose value is being retrieved.
     * @param OutParam The parameter where the value will be set if the optional is set.
     * @return An ESetStatus indicating whether the optional was set or not.
     */
    template <typename T>
        requires BlueprintCompatibleOptional<T>
    constexpr ESetStatus GetBlueprintOptionalValue(T &&Optional, TBlueprintOutParameter<T> &OutParam) {
        if (!Optional.IsSet()) {
            return ESetStatus::NotSet;
        }

        if constexpr (UnrealInterface<TBlueprintOutParameter<T>>) {
            OutParam = std::forward<T>(Optional).GetValue()._getUObject();
        } else if constexpr (std::derived_from<TBlueprintOutParameter<T>, UObject>) {
            OutParam = std::forward<T>(Optional).GetPtrOrNull();
        } else {
            OutParam = std::forward<T>(Optional).GetValue();
        }
        return ESetStatus::IsSet;
    }
} // namespace Retro::Optionals