// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Blueprints/BlueprintRuntimeUtils.h"
#include "RetroLib/Exceptions/VariantException.h"
#include "RetroLib/Functional/Delegates.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "RetroLib/Utils/SoftObjectRef.h"
#include "VariantObject.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * Abstract declaration of the definition of a registered variant struct type. This is meant to only really be used
     * in blueprints as several of this methods throw exception that are intended to be handled by a CustomThunk
     * function that will then trigger an actual blueprint exception.
     */
    class RETROLIB_API IVariantRegistration {
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

        virtual bool IsValidType(const UClass *Class) const = 0;

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
        virtual TOptional<uint64> GetTypeIndex(const TSoftObjectPtr<> &SourceObject) const = 0;

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
        virtual void MakeSoftValue(const TSoftObjectPtr<> &Path, const FStructProperty &SoftProperty,
                                   uint8 *SoftStructValue) const = 0;

        /**
         * Attempt to get the soft object pointer from the given value.
         * @param Class The class to check against
         * @param SoftProperty The soft property to check against
         * @param SoftStructValue The soft struct value in question
         * @return
         */
        virtual TOptional<TSoftObjectRef<>> TryGetSoftValue(const UClass *Class, const FStructProperty &SoftProperty,
                                                            uint8 *SoftStructValue) const = 0;

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

        bool IsValidType(const UClass *Class) const final {
            return T::IsValidType(Class);
        }

        TOptional<uint64> GetTypeIndex(const UObject *SourceObject) const final {
            return T::GetTypeIndex(SourceObject);
        }

        TOptional<uint64> GetTypeIndex(const TSoftObjectPtr<> &SourceObject) const final {
            return T::SoftPtrType::GetTypeIndex(SourceObject);
        }

        void SetStructValue(UObject *SourceObject, const FStructProperty &Property, uint8 *StructValue) const final {
            ValidateStructsMatch(Property, GetStructType());
            auto Variant = std::bit_cast<T *>(StructValue);
            if (auto TypeIndex = T::GetTypeIndex(SourceObject);
                !TypeIndex.IsSet() || TypeIndex.GetValue() == T::template GetTypeIndex<std::nullptr_t>()) {
                throw FVariantException("Incompatible object parameter; the supplied object is not of a "
                    "valid type for this variant object");
            }

            Variant->Set(SourceObject);
        }

        TOptional<UObject &> GetValue(const FStructProperty &Property, uint8 *StructValue) const final {
            ValidateStructsMatch(Property, GetStructType());
            auto Variant = std::bit_cast<T *>(StructValue);
            return Variant->TryGet();
        }

        void MakeSoftValue(const FStructProperty &Property, const uint8 *StructValue,
                           const FStructProperty &SoftProperty, uint8 *SoftStructValue) const final {
            ValidateStructsMatch(Property, GetStructType());
            ValidateStructsMatch(SoftProperty, GetSoftStructType());
            auto Variant = std::bit_cast<const T *>(StructValue);
            auto SoftVariant = std::bit_cast<typename T::SoftPtrType *>(SoftStructValue);

            SoftVariant->Set(*Variant);
        }

        void MakeSoftValue(const TSoftObjectPtr<> &Path, const FStructProperty &SoftProperty,
                           uint8 *SoftStructValue) const final {
            ValidateStructsMatch(SoftProperty, GetSoftStructType());
            auto SoftVariant = std::bit_cast<typename T::SoftPtrType *>(SoftStructValue);
            SoftVariant->Set(Path);
        }

        TOptional<TSoftObjectRef<>> TryGetSoftValue(const UClass *Class, const FStructProperty &SoftProperty,
                                                    uint8 *SoftStructValue) const final {
            ValidateStructsMatch(SoftProperty, GetSoftStructType());
            auto SoftVariant = std::bit_cast<const typename T::SoftPtrType *>(SoftStructValue);
            if (auto ClassIndex = T::GetTypeIndexForClass(Class);
                !ClassIndex.IsSet() || *ClassIndex != SoftVariant->GetTypeIndex()) {
                return TOptional<TSoftObjectRef<>>();
            }

            return TSoftObjectRef<>(SoftVariant->ToSoftObjectPtr());
        }

        bool LoadSynchronous(const FStructProperty &SoftProperty, const uint8 *SoftStructValue,
                             const FStructProperty &Property, uint8 *StructValue) const final {
            ValidateStructsMatch(Property, GetStructType());
            ValidateStructsMatch(SoftProperty, GetSoftStructType());
            auto Variant = std::bit_cast<T *>(SoftStructValue);
            auto SoftVariant = std::bit_cast<const typename T::SoftPtrType *>(SoftStructValue);

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

    class RETROLIB_API IVariantConversion {
    public:
        virtual ~IVariantConversion() = default;

        virtual UScriptStruct *GetSourceStructType() const = 0;

        virtual UScriptStruct *GetSourceSoftStructType() const = 0;

        virtual UScriptStruct *GetDestStructType() const = 0;

        virtual UScriptStruct *GetDestSoftStructType() const = 0;

        virtual bool ConvertVariant(const FStructProperty &SourceProperty, const uint8 *SourceValue,
                                    const FStructProperty &DestProperty, uint8 *DestValue) const = 0;

        virtual bool ConvertSoftVariant(const FStructProperty &SourceProperty, const uint8 *SourceValue,
                                        const FStructProperty &DestProperty, uint8 *DestValue) const = 0;
    };

    template <VariantObjectStruct T, VariantObjectStruct U>
    class TVariantConversionImpl : public IVariantConversion {
    public:
        UScriptStruct *GetSourceStructType() const final {
            return GetScriptStruct<T>();
        }

        UScriptStruct *GetSourceSoftStructType() const final {
            return GetScriptStruct<typename T::SoftPtrType>();
        }

        UScriptStruct *GetDestStructType() const final {
            return GetScriptStruct<U>();
        }

        UScriptStruct *GetDestSoftStructType() const final {
            return GetScriptStruct<typename U::SoftPtrType>();
        }

        bool ConvertVariant(const FStructProperty &SourceProperty, const uint8 *SourceValue,
                            const FStructProperty &DestProperty, uint8 *DestValue) const final {
            ValidateStructsMatch(SourceProperty, GetSourceStructType());
            ValidateStructsMatch(DestProperty, GetDestStructType());
            auto SourcePtr = std::bit_cast<const T *>(SourceValue);
            auto DestPtr = std::bit_cast<U *>(DestValue);

            if (TOptional<U> Converted = SourcePtr->template Convert<U>(); Converted.IsSet()) {
                *DestPtr = Converted.GetValue();
                return true;
            }

            return false;
        }

        bool ConvertSoftVariant(const FStructProperty &SourceProperty, const uint8 *SourceValue,
                                const FStructProperty &DestProperty, uint8 *DestValue) const final {
            ValidateStructsMatch(SourceProperty, GetSourceSoftStructType());
            ValidateStructsMatch(DestProperty, GetDestSoftStructType());
            auto SourcePtr = std::bit_cast<const T *>(SourceValue);
            auto DestPtr = std::bit_cast<U *>(DestValue);

            if (TOptional<U> Converted = SourcePtr->template Convert<U>(); Converted.IsSet()) {
                *DestPtr = std::move(Converted.GetValue());
                return true;
            }

            return false;
        }

    };

    /**
     * Static registry for a variant struct object.
     */
    RETROLIB_EXPORT class RETROLIB_API FVariantObjectStructRegistry {
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
            FCoreDelegates::OnPostEngineInit | Delegates::Add([] {
                auto &Instance = Get();
                auto Struct = GetScriptStruct<T>();
                auto &Registered =
                    Instance.RegisteredStructs.Emplace(Struct->GetFName(), MakeShared<TVariantStructRegistration<T>>());
                // We're using a shared pointer to avoid double allocating identical registration info for the same
                // class
                auto SoftStruct = GetScriptStruct<typename T::SoftPtrType>();
                Instance.RegisteredStructs.Emplace(SoftStruct->GetFName(), Registered);
            });
            return true;
        }

        template <VariantObjectStruct T, VariantObjectStruct U>
        static bool RegisterVariantConversion() {
            FCoreDelegates::OnPostEngineInit | Delegates::Add([] {
                auto &Instance = Get();
                auto SourceStruct = GetScriptStruct<T>();
                auto DestStruct = GetScriptStruct<U>();
                auto &DestMap = Instance.RegisteredConversions.FindOrAdd(SourceStruct->GetFName());
                auto &Registered = DestMap.Emplace(DestStruct->GetFName(), MakeShared<TVariantConversionImpl<T, U>>());
                // We're using a shared pointer to avoid double allocating identical registration info for the same
                // class
                auto SoftSource = GetScriptStruct<typename T::SoftPtrType>();
                auto SoftDest = GetScriptStruct<typename T::SoftPtrType>();
                auto &SoftDestMap = Instance.RegisteredConversions.FindOrAdd(SoftSource->GetFName());
                SoftDestMap.Emplace(SoftDest->GetFName(), Registered);
            });
            return true;
        }

        /**
         * Get the variant struct data for the given struct type.
         * @param Struct The non-null refrence to the struct type
         * @return The registration of the struct (if found)
         */
        TOptional<IVariantRegistration &> GetVariantStructData(const UScriptStruct &Struct);
        
        TOptional<IVariantConversion &> GetVariantStructConversion(const UScriptStruct &From, const UScriptStruct& To);

        /**
         * Get the range of all registered structs.
         * @return The iterable range of all the registered structs.
         */
        auto GetAllRegisteredStructs() const {
            // clang-format off
            return RegisteredStructs |
                   Ranges::Views::Values |
                   Ranges::Views::Transform(&TSharedRef<IVariantRegistration>::operator*);
            // clang-format on
        }

        auto GetAllRegisteredConversions() const {
            // clang-format off
            return RegisteredConversions |
                   Ranges::Views::Values |
                   Ranges::Views::Join |
                   Ranges::Views::Values |
                   Ranges::Views::Transform(&TSharedRef<IVariantConversion>::operator*);
            // clang-format on
        }

    private:
        TMap<FName, TSharedRef<IVariantRegistration>> RegisteredStructs;
        TMap<FName, TMap<FName, TSharedRef<IVariantConversion>>> RegisteredConversions;
    };
} // namespace Retro
#endif