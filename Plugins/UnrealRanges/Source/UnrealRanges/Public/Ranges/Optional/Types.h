// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalRef.h"

/**
 * The optional specific part of the UE Ranges library.
 */
namespace UE::Optionals {

    namespace Detail {

        /**
         * Struct that contains the basic definition for how a return value should be stored in an optional.
         * @tparam T The type to determine the storage of.
         */
        template <typename T>
        struct TOptionalValueTraits {
            /**
             * The basic definition for how a return value should be stored in an optional.
             */
            using Type = T;
        };

        /**
         * Template specialization to indicate that when determining how to store a type into an optional, we need to
         * store it by reference instead.
         * @tparam T The type to determine the storage of.
         */
        template <typename T>
            requires std::is_pointer_v<T> && (!std::same_as<std::remove_cv_t<T>, void>)
        struct TOptionalValueTraits<T> {
            /**
             * The basic definition for how a return value should be stored in an optional.
             */
            using Type = std::remove_pointer_t<T> &;
        };

        /**
         * Template specialization to indicate that when determining how to store a type into an optional, we need to
         * store it as a void value instead.
         * @tparam T The type to determine the storage of.
         */
        template <typename T>
            requires std::same_as<std::remove_cv_t<T>, void>
        struct TOptionalValueTraits<T *> {
            /**
             * The basic definition for how a return value should be stored in an optional.
             */
            using Type = T;
        };

    } // namespace Detail

    /**
     * The basic definition for how a return value should be stored in an optional.
     * @tparam T The type to determine the storage of.
     */
    template <typename T>
    using TOptionalType = typename Detail::TOptionalValueTraits<T>::Type;

} // namespace UE::Optionals
