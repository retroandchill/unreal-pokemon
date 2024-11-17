// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Traits.h"

namespace UE::Optionals {
    /**
     * @class FIsSet
     *
     * @brief FIsSet is a utility class that provides a mechanism for checking if a specific condition is met.
     *
     * This class is designed to perform checks against a given set of criteria, making it useful for validation,
     * constraint checking, and various condition evaluations within an application. It can be extended or composed
     * as needed to fit specific use cases.
     *
     * @details The class encapsulates logic to determine if certain conditions hold true, offering methods
     * to initialize, update, and evaluate these conditions. By abstracting the condition evaluations,
     * it enhances code readability and maintainability, enabling developers to define clear and reusable
     * condition checks.
     */
    struct FIsSet {
        template <typename O>
            requires UEOptional<O>
        constexpr bool operator()(O&& Optional) const {
            return Optional.IsSet();
        }
    };

    /**
     * @brief A constexpr instance of TOptionalClosure initialized with FIsSet.
     *
     * This variable provides a utility for chaining checks if a specific condition is met
     * within an optional type, following the functional programming paradigm.
     * It uses FIsSet to encapsulate the condition-checking logic, making it useful for
     * validation and various condition evaluations within an application.
     *
     * @details As a TOptionalClosure, IsSet enables piping operations on optionals, allowing
     * for a clean and readable way to chain multiple optional manipulations and checks in a
     * manner similar to Java's Optional API. This enhances code readability and maintainability
     * by abstracting the complexity of condition evaluations.
     */
    constexpr TOptionalClosure<FIsSet> IsSet;
}