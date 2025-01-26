// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RetroLib/Ranges/Views/Generator.h"

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
    RETROLIB_EXPORT class FVariantObjectStructRegistry;

    /**
     * Interface for converting data between different variant struct types.
     * Implementations of this interface define the logic for translating data
     * from one struct type to another, supporting both standard and 'soft' variants.
     */
    RETROLIB_EXPORT class RETROLIB_API IVariantConversion {
    public:
        virtual ~IVariantConversion() = default;

        /**
         * Retrieves the source UScriptStruct type for the conversion process.
         * The returned struct type represents the input structure that this
         * conversion interface operates on.
         *
         * @return The source struct type as a UScriptStruct pointer.
         */
        virtual UScriptStruct *GetSourceStructType() const = 0;

        /**
         * Retrieves the source 'soft' UScriptStruct type for the conversion process.
         * This represents a variant of the source struct type used for conversions
         * involving 'soft' variants, allowing flexibility in the struct type handling.
         *
         * @return The source soft struct type as a UScriptStruct pointer.
         */
        virtual UScriptStruct *GetSourceSoftStructType() const = 0;

        /**
         * Retrieves the destination UScriptStruct type for the conversion process.
         * The returned struct type represents the output structure that this
         * conversion interface produces as a result of the conversion.
         *
         * @return The destination struct type as a UScriptStruct pointer.
         */
        virtual UScriptStruct *GetDestStructType() const = 0;

        /**
         * Retrieves the destination 'soft' UScriptStruct type for the conversion process.
         * This represents a variant of the destination struct type used for conversions
         * involving 'soft' variants, providing flexibility in handling the output structure.
         *
         * @return The destination soft struct type as a UScriptStruct pointer.
         */
        virtual UScriptStruct *GetDestSoftStructType() const = 0;

        /**
         * Converts data between two variant struct types.
         * This method defines the logic for translating data from a source struct type to a destination struct type.
         * The conversion can involve both standard and 'soft' variants, ensuring data integrity and type compatibility.
         *
         * @param SourceProperty A reference to the source struct property used in the conversion.
         * @param SourceValue A pointer to the source struct property value.
         * @param DestProperty A reference to the destination struct property where the data will be converted.
         * @param DestValue A pointer to the destination struct property value to store the converted data.
         * @return True if the conversion is successful, otherwise false.
         */
        virtual bool ConvertVariant(const FStructProperty &SourceProperty, const uint8 *SourceValue,
                                    const FStructProperty &DestProperty, uint8 *DestValue) const = 0;

        /**
         * Converts data specifically between 'soft' variant struct types.
         * This method defines the logic for translating data when handling
         * 'soft' variant types, ensuring compatibility and data integrity
         * between the source and destination properties.
         *
         * @param SourceProperty A reference to the source struct property used in the conversion.
         * @param SourceValue A pointer to the source struct property's value.
         * @param DestProperty A reference to the destination struct property for the conversion target.
         * @param DestValue A pointer to the destination struct property's value to store the converted data.
         * @return True if the conversion is successful, otherwise false.
         */
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
     * Abstract declaration of the definition of a registered variant struct type. This is meant to only really be used
     * in blueprints as several of this methods throw exception that are intended to be handled by a CustomThunk
     * function that will then trigger an actual blueprint exception.
     */
    RETROLIB_EXPORT class RETROLIB_API IVariantRegistration {
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

        /**
         * Attempts to retrieve a conversion interface for transforming data to the specified target struct type.
         * Implementations may provide a specialized conversion logic for handling variant struct data.
         *
         * @param To The target UScriptStruct type to which the data should be converted.
         * @return An optional reference to an IVariantConversion object if a valid conversion exists, or an empty optional otherwise.
         */
        virtual TOptional<IVariantConversion &> GetConversion(const UScriptStruct &To) const = 0;

        /**
         * Retrieves all available variant conversions.
         * Combines multiple data processing views to extract and transform conversion objects
         * into their appropriate shared reference variant type.
         */
        auto GetAllConversions() const {
            return Conversions |
                Ranges::Views::Values |
                    Ranges::Views::Transform(&TSharedRef<IVariantConversion>::Get);
        }
    
    protected:
        /**
         * Retrieves the map of variant conversions.
         * This map associates FName keys with shared references to IVariantConversion,
         * enabling the management and retrieval of conversion logic between variant types.
         *
         * @return A reference to the map containing variant conversions.
         */
        TMap<FName, TSharedRef<IVariantConversion>> &GetMutableConversions() {
            return Conversions;
        }

        /**
         * Retrieves the map of variant conversions.
         * This map associates FName keys with shared references to IVariantConversion,
         * enabling the management and retrieval of conversion logic between variant types.
         *
         * @return A reference to the map containing variant conversions.
         */
        const TMap<FName, TSharedRef<IVariantConversion>> &GetConversions() const {
            return Conversions;
        }

    private:
        friend class FVariantObjectStructRegistry;
        
        TMap<FName, TSharedRef<IVariantConversion>> Conversions;
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

        TOptional<IVariantConversion &> GetConversion(const UScriptStruct &To) const final {
            return Optionals::OfNullable(GetConversions().Find(To.GetFName())) |
                   Optionals::Transform(&TSharedRef<IVariantConversion>::Get);
        }
    };

    /**
     * Static registry for a variant struct object.
     */
    class RETROLIB_API FVariantObjectStructRegistry {
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

        /**
         * Registers a variant conversion between two struct types for the variant object system.
         * This function defers the registration until the `OnPostEngineInit` event is triggered.
         *
         * It ensures that both direct and 'soft' variant conversions for a given struct are properly
         * added to the registry. The function is typically used at compile-time for setting up
         * conversions between specific struct types.
         *
         * @return True if the registration process completes successfully (always returns true).
         */
        template <VariantObjectStruct T, VariantObjectStruct U>
        static bool RegisterVariantConversion() {
            FCoreDelegates::OnPostEngineInit | Delegates::Add([] {
                auto &Instance = Get();
                auto SourceStruct = GetScriptStruct<T>();
                auto &DestRegistration = Instance.RegisteredStructs.FindChecked(SourceStruct->GetFName());
                auto &DestMap = DestRegistration->GetMutableConversions();
                auto &Registered = DestMap.Emplace(SourceStruct->GetFName(),
                                                   MakeShared<TVariantConversionImpl<T, U>>());
                // We're using a shared pointer to avoid double allocating identical registration info for the same
                // class
                auto SoftSource = GetScriptStruct<typename T::SoftPtrType>();
                DestMap.Emplace(SoftSource->GetFName(), Registered);
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
         * Retrieves a variant struct conversion interface for converting data between two specified UScriptStructs.
         * This method attempts to find a conversion that can translate data from one struct type to another
         * within the registry of variant object structures.
         *
         * @param From The source UScriptStruct type to convert from.
         * @param To The target UScriptStruct type to convert to.
         * @return An optional conversion interface capable of handling the transformation, or an empty optional if no conversion is available.
         */
        TOptional<IVariantConversion &> GetVariantStructConversion(const UScriptStruct &From, const UScriptStruct &To);

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

        /**
         * Retrieves all registered variant conversions from the registry.
         * This method aggregates all the variant conversions defined for the registered
         * structs and returns a collection of shared references to the conversion interfaces.
         *
         * The pipeline combines multiple operations such as aggregating conversions,
         * flattening nested collections, extracting values, and dereferencing shared references.
         *
         * @return A range containing all the registered variant conversion interfaces as dereferenced shared references.
         */
        auto GetAllRegisteredConversions() const {
            // clang-format off
            return GetAllRegisteredStructs() |
                   Ranges::Views::Transform(&IVariantRegistration::GetMutableConversions) |
                   Ranges::Views::Join |
                   Ranges::Views::Values |
                   Ranges::Views::Transform(&TSharedRef<IVariantConversion>::operator*);
            // clang-format on
        }

    private:
        TMap<FName, TSharedRef<IVariantRegistration>> RegisteredStructs;
    };
} // namespace Retro
#endif