// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Variants/SoftVariantObject.h"
#include "Ranges/Optional/OptionalRef.h"

namespace UE::Ranges {
    class UNREALRANGES_API FVariantObjectStructRegistry {
        FVariantObjectStructRegistry() = default;
        ~FVariantObjectStructRegistry() = default;

    public:
        static FVariantObjectStructRegistry& Get();

        template <typename T>
            requires VariantObjectStruct<T>
        static bool RegisterVariantStruct() {
            AddToDelegate(FCoreDelegates::OnPostEngineInit, [] {
                auto &Instance = Get();
                auto Struct = GetScriptStruct<T>();
                Instance.RegisteredStructs.Emplace(Struct->GetFName(),
                    GetScriptStruct<typename T::SoftPtrType>());
            });
            return true;
        }

        TOptional<UScriptStruct&> GetStruct(const UScriptStruct &StructName);

    private:
        TMap<FName, TWeakObjectPtr<UScriptStruct>> RegisteredStructs;
    };
}

#define UE_DECLARE_VARIANT_OBJECT_STRUCT(StructName, ...)                                                              \
    struct FSoft##StructName;                                                                                          \
    struct F##StructName : UE::Ranges::TVariantObject<__VA_ARGS__> {                                                   \
        using SoftPtrType = FSoft##StructName;                                                                         \
        F##StructName() = default;                                                                                     \
        template <typename... T>                                                                                       \
            requires std::constructible_from<TVariantObject, T...>                                                     \
        explicit F##StructName(T &&...Args)               \
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
        explicit FSoft##StructName(T &&...Args)       \
            : TSoftVariantObject(Forward<T>(Args)...) {                                                                \
        }                                                                                                              \
    };                                                                                                                 \
    template <>                                                                                                        \
    struct UE::Ranges::Detail::TIsSoftVariantObject<FSoft##StructName> : std::true_type {}

#define UE_DEFINE_VARIANT_OBJECT_STRUCT(StructName) \
    static const bool __##StructName__Registration = UE::Ranges::FVariantObjectStructRegistry::RegisterVariantStruct<StructName>()