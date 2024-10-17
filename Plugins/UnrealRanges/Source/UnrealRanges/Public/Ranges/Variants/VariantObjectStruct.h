// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/BlueprintException.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Variants/SoftVariantObject.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Views/MapValue.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Span.h"

#include <bit>

#include "VariantObjectStruct.generated.h"

namespace UE::Ranges {
    class UNREALRANGES_API IVariantRegistration {
    public:
        virtual ~IVariantRegistration() = default;

        virtual UScriptStruct *GetStructType() const = 0;
        virtual UScriptStruct *GetSoftStructType() const = 0;
        virtual TOptional<uint64> GetTypeIndex(const UObject* SourceObject) const = 0;
        virtual void SetStructValue(UObject *SourceObject, const FStructProperty &Property,
                                    uint8 *StructValue) const = 0;
        virtual TOptional<UObject&> GetValue(const FStructProperty &Property, uint8 *StructValue) const = 0;
        virtual TSpan<UClass*> GetValidClasses() const = 0;

        auto GetClassesWithStructType() const {
            auto StructType = GetStructType();
            return GetValidClasses() |
                Map([StructType](UClass* Class) { return std::make_pair(StructType, Class); });
        }
    };

    template <typename T>
        requires VariantObjectStruct<T>
    class TVariantStructRegistration : public IVariantRegistration {
    public:
        UScriptStruct *GetStructType() const final {
            return GetScriptStruct<T>();
        }

        UScriptStruct *GetSoftStructType() const final {
            return GetScriptStruct<typename T::SoftPtrType>();
        }

        TOptional<uint64> GetTypeIndex(const UObject* SourceObject) const {
            return T::GetTypeIndex(SourceObject);
        }

        void SetStructValue(UObject *SourceObject, const FStructProperty &Property, uint8 *StructValue) const final {
            if (Property.Struct != GetStructType()) {
                throw FBlueprintException(EBlueprintExceptionType::AccessViolation,
                                          NSLOCTEXT(
                                              "SetStructValue", "IncompatibleProperty",
                                              "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *VariantPtr = StructValue;
            auto Variant = static_cast<T *>(VariantPtr);
            auto TypeIndex = T::GetTypeIndex(SourceObject);
            if (!TypeIndex.IsSet() || TypeIndex.GetValue() == T::template GetTypeIndex<std::nullptr_t>()) {
                throw FBlueprintException(EBlueprintExceptionType::AccessViolation,
                                          NSLOCTEXT("CreateVariantFromObject", "InvalidObjectType",
                                                    "Incompatible object parameter; the supplied object is not of a valid type for this variant object"));
            }

            Variant->Set(SourceObject);
        }
        
        TOptional<UObject&> GetValue(const FStructProperty &Property, uint8 *StructValue) const {
            if (Property.Struct != GetStructType()) {
                throw FBlueprintException(EBlueprintExceptionType::AccessViolation,
                                          NSLOCTEXT(
                                              "SetStructValue", "IncompatibleProperty",
                                              "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *VariantPtr = StructValue;
            auto Variant = static_cast<T *>(VariantPtr);
            return Variant->TryGet();
        }

        TSpan<UClass*> GetValidClasses() const final {
            return T::GetTypeClasses();
        }
    };

    class UNREALRANGES_API FVariantObjectStructRegistry {
        FVariantObjectStructRegistry() = default;
        ~FVariantObjectStructRegistry() = default;

    public:
        FVariantObjectStructRegistry(const FVariantObjectStructRegistry &) = delete;
        FVariantObjectStructRegistry(FVariantObjectStructRegistry &&) = delete;

        FVariantObjectStructRegistry &operator=(const FVariantObjectStructRegistry &) = delete;
        FVariantObjectStructRegistry &operator=(FVariantObjectStructRegistry &&) = delete;
        
        static FVariantObjectStructRegistry &Get();

        template <typename T>
            requires VariantObjectStruct<T>
        static bool RegisterVariantStruct() {
            AddToDelegate(FCoreDelegates::OnPostEngineInit, [] {
                auto &Instance = Get();
                auto Struct = GetScriptStruct<T>();
                Instance.RegisteredStructs.Emplace(Struct->GetFName(),
                                                   MakeUnique<TVariantStructRegistration<T>>());
            });
            return true;
        }

        TOptional<IVariantRegistration &> GetVariantStructData(const UScriptStruct &Struct);

        auto GetAllRegisteredStructs() const {
            return RegisteredStructs |
                   MapValue |
                   Map(&TUniquePtr<IVariantRegistration>::operator*);
        }

    private:
        TMap<FName, TUniquePtr<IVariantRegistration>> RegisteredStructs;
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