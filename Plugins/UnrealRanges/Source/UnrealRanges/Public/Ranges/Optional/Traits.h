// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Templates/NonNullSubclassOf.h"

namespace UE::Optionals {

    namespace Detail {

        /**
         * General definition to define most types as not an optional.
         */
        template <typename>
        struct TOptionalTraits;

        /**
         * Definition used to define TOptional as a valid optional type.
         * @tparam T The type contained within the optional.
         */
        template <typename T>
        struct TOptionalTraits<TOptional<T>> {

            /**
             * The actual type specified in the type parameter of the optional.
             */
            using ContainedType = T;

            /**
             * The effective contained type of the optional.
             */
            using ElementType = typename TOptional<T>::ElementType;
        };

        template <typename T>
        struct TOptionalTraits<TOptional<TNonNullSubclassOf<T>>> {
            /**
             * The actual type specified in the type parameter of the optional.
             */
            using ContainedType = TNonNullSubclassOf<T>;

            /**
             * The effective contained type of the optional.
             */
            using ElementType = TSubclassOf<T>;
        };

    } // namespace Detail

    /**
     * Concept that checks if a type is a TOptional type.
     * @tparam T The type to check
     */
    template <typename T>
    concept UEOptional = requires { typename Detail::TOptionalTraits<std::remove_cvref_t<T>>::ContainedType; };

    template <typename T>
    concept SubclassOptional = UEOptional<T> && requires(T &&Optional) {
        { Optional.Get(nullptr) } -> std::same_as<UClass *>;
    };

    /**
     * Type definition to get the underlying type out of the optional
     * @tparam T The optional type to get the contained type from
     */
    template <typename T>
        requires UEOptional<T>
    using TContainedOptionalType = typename Detail::TOptionalTraits<std::remove_cvref_t<T>>::ContainedType;

    /**
     * Type definition to get the underlying element type out of the optional
     * @tparam T The optional type to get the element type from
     */
    template <typename T>
        requires UEOptional<T>
    using TOptionalElementType = typename Detail::TOptionalTraits<std::remove_cvref_t<T>>::ElementType;

    static_assert(SubclassOptional<TOptional<TNonNullSubclassOf<UObject>>>);

} // namespace UE::Optionals