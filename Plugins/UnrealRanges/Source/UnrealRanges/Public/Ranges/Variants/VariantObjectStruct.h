// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Variants/SoftVariantObject.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Views/MapValue.h"
#include "Ranges/Views/ContainerView.h"

#include "VariantObjectStruct.generated.h"

namespace UE::Ranges {
    class UNREALRANGES_API FRegisteredVariantStruct {
        FRegisteredVariantStruct(UScriptStruct *StructType,
            UScriptStruct* SoftStructType,
            TOptional<uint64> (*IndexFunction) (const UObject*),
            TArray<UClass*>&& AllowedClasses);

    public:
        template <typename T>
            requires VariantObjectStruct<T>
        static FRegisteredVariantStruct Create() {
            return FRegisteredVariantStruct(GetScriptStruct<T>(),
                GetScriptStruct<typename T::SoftPtrType>(),
                &T::GetTypeIndex,
                T::GetTypeClasses());
        }
        
        UScriptStruct* GetStructType() const {
            return StructType;
        }

        UScriptStruct* GetSoftStructType() const {
            return SoftStructType;
        }

        TOptional<uint64> GetTypeIndex(const UObject* Object) const {
            return TypeIndexFunction(Object);
        }

        const TArray<UClass*>& GetAllowedClasses() const {
            return AllowedClasses;
        }

        auto GetClassesWithStructType() const {
            return AllowedClasses |
                Map([this](UClass* Class) { return TPair<UScriptStruct*, UClass*>(StructType, Class); });
        }

    private:
        UScriptStruct* StructType;
        UScriptStruct* SoftStructType;
        TOptional<uint64> (*TypeIndexFunction) (const UObject*);
        TArray<UClass*> AllowedClasses;
    };
    
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
                    FRegisteredVariantStruct::Create<T>());
            });
            return true;
        }
        
        TOptional<FRegisteredVariantStruct&> GetVariantStructData(const UScriptStruct &Struct);

        auto GetAllRegisteredStructs() const {
            return RegisteredStructs | MapValue;
        }

    private:
        TMap<FName, FRegisteredVariantStruct> RegisteredStructs;
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

USTRUCT()
struct UNREALRANGES_API FVariantObjectTemplate {
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};

USTRUCT()
struct UNREALRANGES_API FSoftVariantObjectTemplate {
    GENERATED_BODY()
    
    UPROPERTY()
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
};