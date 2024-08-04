// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

template <typename T>
concept overloads_equals = requires(T A, T B) { A == B; };

namespace OptionalUtilities {
template <typename T>
TOptional<T> OfNullable(const T* Value) {
    return Value != nullptr ? TOptional<T>(*Value) : TOptional<T>();
}

/**
 * Construct an optional from an overriden UPROPERTY value
 * @tparam T The data type of the optional
 * @param Property The value of the property
 * @param bOverride Is the property being overriden.
 * @return An optional containing the value if bOverride is true, otherwise an empty optional
 */
template <typename T>
TOptional<T> CreateOptional(const T &Property, bool bOverride) {
    return bOverride ? TOptional<T>(Property) : TOptional<T>();
}

/**
 * Construct an optional from an overriden UPROPERTY value
 * @tparam T The data type of the optional
 * @param Property The value of the property
 * @param bOverride Is the property being overriden.
 * @return An optional containing the value if bOverride is true, otherwise an empty optional
 */
template <typename T>
TOptional<T> CreateOptional(T &&Property, bool bOverride) {
    return bOverride ? TOptional<T>(Forward(Property)) : TOptional<T>();
}

/**
 * Get the value of the optional of call to the calling functor
 * @tparam T The type stored within the optional
 * @tparam Functor The type of callback function
 * @param Optional The optional to check
 * @param Supplier The callback function
 * @return The optional's value or the default supplied on
 */
template <typename T, typename Functor>
    requires std::is_invocable_r_v<T, Functor>
T OrElseGet(const TOptional<T> &Optional, const Functor &Supplier) {
    return Optional.IsSet() ? Optional.GetValue() : Supplier();
}

template <typename R, typename T>
TOptional<R> Map(const TOptional<T> &Optional, const TFunctionRef<R(const T &)> &Mapping) {
    return Optional.IsSet() ? Mapping(Optional.GetValue()) : TOptional<R>();
}

/**
 * Compare two optionals to see if they're equal
 * @tparam T The type stored within the optionals
 * @param A The first optional to compare
 * @param B The second optional to compare
 * @return They they equal?
 */
template <overloads_equals T>
bool OptionalsSame(const TOptional<T> &A, const TOptional<T> &B) {
    if (A.IsSet() != B.IsSet())
        return false;

    return A.IsSet() ? A.GetValue() == B.GetValue() : true;
}

/**
 * Compare two optionals to see if they're equal
 * @param A The first optional to compare
 * @param B The second optional to compare
 * @return They they equal?
 */
POKEMONUTILITIES_API bool OptionalsSame(const TOptional<FText> &A, const TOptional<FText> &B);
} // namespace OptionalUtilities

#define OPTIONAL(OwningObject, Property) CreateOptional((OwningObject).Property, (OwningObject).bOverride_##Property)

#define BOOL_OPTIONAL(OwningObject, Property)                                                                          \
    CreateOptional((OwningObject).b##Property, (OwningObject).bOverride_##Property)