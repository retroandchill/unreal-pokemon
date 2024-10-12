// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/range/concepts.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {
    namespace Detail {

        /**
         * Type trait to signify if a container is a UE container of some kind.
         */
        template <typename>
        struct TIsUEContainer : std::false_type {};

        /**
         * Template specialization for TArray so that a range can be made from a reference to one.
         * @tparam T The type of data held by the array.
         * @tparam A The allocator used for the array
         */
        template <typename T, typename A>
        struct TIsUEContainer<TArray<T, A>> : std::true_type {
            using ValueType = T;
        };

        /**
         * Template specialization for TArrayView so that a range can be made from a reference to one.
         * @tparam T The type of data held by the array.
         * @tparam S The size type of the array being viewed.
         */
        template <typename T, typename S>
        struct TIsUEContainer<TArrayView<T, S>> : std::true_type {
            using ValueType = T;
        };

        /**
         * Template specialization for TSet so that a range can be made from a reference to one.
         * @tparam T The type of data held by the set.
         * @tparam K The type of the key function.
         * @tparam A The type of the allocator for the set
         */
        template <typename T, typename K, typename A>
        struct TIsUEContainer<TSet<T, K, A>> : std::true_type {
            using ValueType = T;
        };

        /**
         * Template specialization for TMap so that a range can be made from a reference to one.
         * @tparam K The type of the map's keys
         * @tparam V The type of the map's values
         * @tparam A The type of the allocator for the map
         * @tparam F The type of the key function.
         */
        template <typename K, typename V, typename A, typename F>
        struct TIsUEContainer<TMap<K, V, A, F>> : std::true_type {
            using ValueType = TPair<K, V>;
        };

        template <typename>
        struct TIsUEMap : std::false_type {};

        template <typename K, typename V, typename A, typename F>
        struct TIsUEMap<TMap<K, V, A, F>> : std::true_type {};

    } // namespace Detail

    /**
     * Concept used to check if the passed type is a valid UE container.
     * @tparam T The type to check against.
     */
    template <typename T>
    concept UEContainer = Detail::TIsUEContainer<std::remove_cvref_t<T>>::value;

    template <typename T>
        requires UEContainer<T>
    using TValueType = typename Detail::TIsUEContainer<std::remove_cvref_t<T>>::ValueType;

    template <typename T>
    concept UEMap = Detail::TIsUEMap<T>::value;

} // namespace UE::Ranges