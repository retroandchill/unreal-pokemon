// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SoftVariantObject.h"
#include "VariantObject.h"

#define UE_DECLARE_VARIANT_OBJECT_STRUCT(StructName, ...)                                                              \
    struct FSoft##StructName;                                                                                          \
    struct F##StructName : UE::Ranges::TVariantObject<__VA_ARGS__> {                                                   \
        using SoftPtrType = FSoft##StructName;                                                                         \
        F##StructName() = default;                                                                                     \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TVariantObject, T...>                                                     \
        explicit(std::same_as<TVariantObject, std::remove_reference_t<T>...>) F##StructName(T &&...Args)               \
            : TVariantObject(Forward<T>(Args)...) {                                                                    \
        }                                                                                                              \
        void Reset() {                                                                                                 \
            SetUnchecked(nullptr);                                                                                     \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct UE::Ranges::Detail::TIsVariantObject<F##StructName> : std::true_type {};                                    \
    struct FSoft##StructName : UE::Ranges::TSoftVariantObject<F##StructName> {                                         \
        FSoft##StructName() = default;                                                                                 \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TSoftVariantObject, T...>                                                 \
        explicit(std::same_as<TSoftVariantObject, std::remove_reference_t<T>...>) FSoft##StructName(T &&...Args)       \
            : TSoftVariantObject(Forward<T>(Args)...) {                                                                \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct UE::Ranges::Detail::TIsSoftVariantObject<FSoft##StructName> : std::true_type {}