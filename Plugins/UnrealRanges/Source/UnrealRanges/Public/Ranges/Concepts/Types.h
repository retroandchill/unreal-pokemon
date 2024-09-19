// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Views/ContainerView.h"

namespace UE::Ranges {

    namespace Detail {
        template <typename>
        struct TRangeTraits;

        template <typename T>
            requires ranges::range<T>
        struct TRangeTraits<T> {
            using Value = ranges::range_value_t<T>;
            using CommonReference = ranges::range_common_reference_t<T>;
            using Reference = ranges::range_reference_t<T>;
        };

        template <typename T>
            requires UEContainer<T>
        struct TRangeTraits<T> {
            using Value = typename TRangeTraits<TUEContainerView<std::remove_reference_t<T>>>::Value;
            using CommonReference =
                typename TRangeTraits<TUEContainerView<std::remove_reference_t<T>>>::CommonReference;
            using Reference = typename TRangeTraits<TUEContainerView<std::remove_reference_t<T>>>::Reference;
        };
    } // namespace Detail

    template <typename T>
    concept Range = UEContainer<T> || ranges::range<T>;

    template <typename T>
        requires Range<T>
    using TRangeValue = typename Detail::TRangeTraits<T>::Value;

    template <typename T>
        requires Range<T>
    using TRangeReference = typename Detail::TRangeTraits<T>::Reference;

    template <typename T>
        requires Range<T>
    using TRangeCommonReference = typename Detail::TRangeTraits<T>::CommonReference;

} // namespace UE::Ranges
