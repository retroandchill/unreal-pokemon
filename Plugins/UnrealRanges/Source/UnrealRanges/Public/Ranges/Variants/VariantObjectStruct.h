// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/TypeException.h"
#include "Ranges/Exceptions/VariantException.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Variants/SoftVariantObject.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Pointers/SoftObjectRef.h"
#include "Ranges/Views/CacheLast.h"
#include "Ranges/Views/MapValue.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Span.h"

namespace UE::Ranges {
    /**
     * Abstract declaration of the definition of a registered variant struct type. This is meant to only really be used
     * in blueprints as several of this methods throw exception that are intended to be handled by a CustomThunk function
     * that will then trigger an actual blueprint exception.
     */
    class UNREALRANGES_API IVariantRegistration {
    public:
        virtual ~IVariantRegistration() = default;

        /**
         * Get the base struct for the given registration.
         * @return The base struct for the given registration.
         */
        virtual UScriptStruct *GetStructType() const = 0;

        /**
         * The struct used to represent a soft reference to this struct.
         * @return The struct used to represent a soft reference to this struct.
         */
        virtual UScriptStruct *GetSoftStructType() const = 0;

        virtual bool IsValidType(const UClass* Class) const = 0;

        /**
         * Get the type index for the given object
         * @param SourceObject The object to check against
         * @return The type index of the object
         */
        virtual TOptional<uint64> GetTypeIndex(const UObject *SourceObject) const = 0;

        /**
         * Get the type index for the given object
         * @param SourceObject The object to check against
         * @return The type index of the object
         */
        virtual TOptional<uint64> GetTypeIndex(const TSoftObjectPtr<>& SourceObject) const = 0;

        /**
         * Set the value of the given struct to the given source object.
         * @param SourceObject The object in question
         * @param Property The property information for the struct
         * @param StructValue The pointer to the raw bytes of the struct
         * @throws FTypeException If the supplied struct is not the same type as this registration
         * @throws FVariantException If the supplied object is not valid for the given variant
         */
        virtual void SetStructValue(UObject *SourceObject, const FStructProperty &Property,
                                    uint8 *StructValue) const = 0;

        /**
         * Get the underlying value of the variant
        * @param Property The property information for the struct
         * @param StructValue The pointer to the raw bytes of the struct
         * @return The object for the variant (if found)
         * @throws FTypeException If the supplied struct is not the same type as this registration
         */
        virtual TOptional<UObject &> GetValue(const FStructProperty &Property, uint8 *StructValue) const = 0;

        /**
         * Make a soft pointer value from the given hard reference.
         * @param Property The property for the hard reference.
         * @param StructValue The pointer to the raw bytes of the hard reference
         * @param SoftProperty The property for the soft reference.
         * @param SoftStructValue The pointer to the raw bytes of the soft reference
         * @throws FTypeException If the supplied structs are not the same type as this registration
         */
        virtual void MakeSoftValue(const FStructProperty &Property, const uint8 *StructValue,
                                   const FStructProperty &SoftProperty, uint8 *SoftStructValue) const = 0;

        /**
         * Make a soft pointer value from the given hard reference.
         * @param Path The soft path to set from
         * @param SoftProperty The property for the soft reference.
         * @param SoftStructValue The pointer to the raw bytes of the soft reference
         * @throws FTypeException If the supplied structs are not the same type as this registration
         */
        virtual void MakeSoftValue(const TSoftObjectPtr<> &Path,
                                   const FStructProperty &SoftProperty, uint8 *SoftStructValue) const = 0;

        /**
         * Attempt to get the soft object pointer from the given value.
         * @param Class The class to check against
         * @param SoftProperty The soft property to check against
         * @param SoftStructValue The soft struct value in question
         * @return 
         */
        virtual TOptional<TSoftObjectRef<>> TryGetSoftValue(const UClass* Class, const FStructProperty &SoftProperty, uint8 *SoftStructValue) const = 0;

        /**
         * Synchronously load the object contained within the struct.
         * @param SoftProperty The property for the soft reference.
         * @param SoftStructValue The pointer to the raw bytes of the soft reference
         * @param Property The property for the hard reference.
         * @param StructValue The pointer to the raw bytes of the hard reference
         */
        virtual bool LoadSynchronous(const FStructProperty &SoftProperty, const uint8 *SoftStructValue,
                                     const FStructProperty &Property, uint8 *StructValue) const = 0;

        /**
         * Get the span of valid classes for the object
         * @return The span of classes
         */
        virtual TArray<UClass *> GetValidClasses() const = 0;
    };

    /**
     * The implementation for the registration of an individual struct. This is stored as a pointer to an interface so
     * the type information ends up being erased.
     * @tparam T The type of the struct in question
     */
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

        bool IsValidType(const UClass* Class) const final {
            return T::IsValidType(Class);
        }

        TOptional<uint64> GetTypeIndex(const UObject *SourceObject) const final {
            return T::GetTypeIndex(SourceObject);
        }

        TOptional<uint64> GetTypeIndex(const TSoftObjectPtr<>& SourceObject) const final {
            return T::SoftPtrType::GetTypeIndex(SourceObject);
            
        }

        void SetStructValue(UObject *SourceObject, const FStructProperty &Property, uint8 *StructValue) const final {
            if (Property.Struct != GetStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "SetStructValue", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *VariantPtr = StructValue;
            auto Variant = static_cast<T *>(VariantPtr);
            auto TypeIndex = T::GetTypeIndex(SourceObject);
            if (!TypeIndex.IsSet() || TypeIndex.GetValue() == T::template GetTypeIndex<std::nullptr_t>()) {
                throw FVariantException(EBlueprintExceptionType::AccessViolation,
                                        NSLOCTEXT("CreateVariantFromObject", "InvalidObjectType",
                                                  "Incompatible object parameter; the supplied object is not of a valid type for this variant object"));
            }

            Variant->Set(SourceObject);
        }

        TOptional<UObject &> GetValue(const FStructProperty &Property, uint8 *StructValue) const final {
            if (Property.Struct != GetStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "SetStructValue", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *VariantPtr = StructValue;
            auto Variant = static_cast<T *>(VariantPtr);
            return Variant->TryGet();
        }

        void MakeSoftValue(const FStructProperty &Property, const uint8 *StructValue,
                           const FStructProperty &SoftProperty, uint8 *SoftStructValue) const final {
            if (Property.Struct != GetStructType() || SoftProperty.Struct != GetSoftStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "MakeSoftValue", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            const void *VariantPtr = StructValue;
            auto Variant = static_cast<const T *>(VariantPtr);

            void *SoftVariantPtr = SoftStructValue;
            auto SoftVariant = static_cast<typename T::SoftPtrType *>(SoftVariantPtr);

            SoftVariant->Set(*Variant);
        }

        void MakeSoftValue(const TSoftObjectPtr<> &Path,
                           const FStructProperty &SoftProperty, uint8 *SoftStructValue) const final {
            if (SoftProperty.Struct != GetSoftStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "MakeSoftValue", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *SoftVariantPtr = SoftStructValue;
            auto SoftVariant = static_cast<typename T::SoftPtrType *>(SoftVariantPtr);
            SoftVariant->Set(Path);
        }

        TOptional<TSoftObjectRef<>> TryGetSoftValue(const UClass* Class, const FStructProperty &SoftProperty, uint8 *SoftStructValue) const {
            if (SoftProperty.Struct != GetSoftStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "LoadSynchronous", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            const void *SoftVariantPtr = SoftStructValue;
            auto SoftVariant = static_cast<const typename T::SoftPtrType *>(SoftVariantPtr);
            auto ClassIndex = T::GetTypeIndexForClass(Class);
            if (!ClassIndex.IsSet() || *ClassIndex != SoftVariant->GetTypeIndex()) {
                return TOptional<TSoftObjectRef<>>();
            }

            return TSoftObjectRef<>(SoftVariant->ToSoftObjectPtr());
        }

        bool LoadSynchronous(const FStructProperty &SoftProperty, const uint8 *SoftStructValue,
                                     const FStructProperty &Property, uint8 *StructValue) const {
            if (Property.Struct != GetStructType() || SoftProperty.Struct != GetSoftStructType()) {
                throw FTypeException(EBlueprintExceptionType::AccessViolation,
                                     NSLOCTEXT(
                                         "LoadSynchronous", "IncompatibleProperty",
                                         "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
            }

            void *VariantPtr = StructValue;
            auto Variant = static_cast<T *>(VariantPtr);

            const void *SoftVariantPtr = SoftStructValue;
            auto SoftVariant = static_cast<const typename T::SoftPtrType *>(SoftVariantPtr);

            auto Result = SoftVariant->LoadSynchronous();
            if (!Result.IsSet()) {
                return false;
            }
            
            *Variant = Result.GetValue();
            return true;
        }

        TArray<UClass *> GetValidClasses() const final {
            return T::GetTypeClasses();
        }
    };

    /**
     * Static registry for a variant struct object.
     */
    class UNREALRANGES_API FVariantObjectStructRegistry {
        FVariantObjectStructRegistry() = default;
        ~FVariantObjectStructRegistry() = default;

    public:
        FVariantObjectStructRegistry(const FVariantObjectStructRegistry &) = delete;
        FVariantObjectStructRegistry(FVariantObjectStructRegistry &&) = delete;

        FVariantObjectStructRegistry &operator=(const FVariantObjectStructRegistry &) = delete;
        FVariantObjectStructRegistry &operator=(FVariantObjectStructRegistry &&) = delete;

        /**
         * Get the static instance of the registry.
         * @return The static instance of the registry.
         */
        static FVariantObjectStructRegistry &Get();

        /**
         * Register a new struct with this registry. Actual registration is deferred until OnPostEngineInit is fired.
         * Once that event has been triggered this function does nothing.
         * @tparam T The type of struct to register.
         * @return If the registration was successful (always true, used to enable compile time registration)
         */
        template <typename T>
            requires VariantObjectStruct<T>
        static bool RegisterVariantStruct() {
            AddToDelegate(FCoreDelegates::OnPostEngineInit, [] {
                auto &Instance = Get();
                auto Struct = GetScriptStruct<T>();
                auto &Registered = Instance.RegisteredStructs.Emplace(Struct->GetFName(),
                                                   MakeShared<TVariantStructRegistration<T>>());
                // We're using a shared pointer to avoid double allocating identical registration info for the same class
                auto SoftStruct = GetScriptStruct<typename T::SoftPtrType>();
                Instance.RegisteredStructs.Emplace(SoftStruct->GetFName(), Registered);
            });
            return true;
        }

        /**
         * Get the variant struct data for the given struct type.
         * @param Struct The non-null refrence to the struct type
         * @return The registration of the struct (if found)
         */
        TOptional<IVariantRegistration &> GetVariantStructData(const UScriptStruct &Struct);

        /**
         * Get the range of all registered structs.
         * @return The iterable range of all the registered structs.
         */
        auto GetAllRegisteredStructs() const {
            return RegisteredStructs |
                   MapValue |
                   Map(&TSharedRef<IVariantRegistration>::operator*);
        }

    private:
        TMap<FName, TSharedRef<IVariantRegistration>> RegisteredStructs;
    };
}

/**
 * Declare a new variant object struct with the given name
 * @param StructName The name of the struct in question
 * @param ... The types that are registered to the struct type
 */
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

/**
 * Perform the static registration of the struct type. This is required to allow a variant struct to be accessible to blueprints.
 * @param StructName The name of the struct to implement.
 */
#define UE_DEFINE_VARIANT_OBJECT_STRUCT(StructName) \
    static const bool __##StructName__Registration = UE::Ranges::FVariantObjectStructRegistry::RegisterVariantStruct<StructName>()