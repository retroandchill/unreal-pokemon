// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#ifdef __UNREAL__
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Concepts/Interfaces.h"
#include "RetroLib/Concepts/Structs.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    RETROLIB_EXPORT template <typename... T>
        requires((std::is_base_of_v<UObject, T> || UnrealInterface<T>) && ...)
    struct TVariantObject;
    
        template <typename>
        struct TIsVariantObject : std::false_type {};

        template <typename... T>
        struct TIsVariantObject<TVariantObject<T...>> : std::true_type {};

    /**
     * Checks if the given type is a variant object.
     * @tparam T The type to check
     */
    template <typename T>
    concept VariantObject = TIsVariantObject<T>::value;

    /**
     * Checks if the given type is a variant object type that has a valid UStruct representation..
     * @tparam T The type to check
     */
    template <typename T>
    concept VariantObjectStruct = UEStruct<T> && VariantObject<T> && requires { typename T::SoftPtrType; };

    template <typename T>
        requires VariantObjectStruct<T>
    class TVariantObjectCustomization;

    /**
     * Specialized variant type that handles a subset of UObject subclasses and interfaces.
     * @tparam T The types that are valid
     */
    template <typename... T>
        requires((std::is_base_of_v<UObject, T> || UnrealInterface<T>) && ...)
    struct TVariantObject {
        static constexpr bool bHasIntrusiveUnsetOptionalState = true;
        using IntrusiveUnsetOptionalStateType = TVariantObject;

        /**
         * Default construct, creates a variant object that exists within an invalid state,
         * where the contained value is nullptr. If a TOptional<> is constructed from this state, it will construct
         * into the unset state.
         */
        TVariantObject() = default;

        /**
         * Construct a new object from the given object
         * @tparam U The type of object to construct from
         * @param Object The object to construct from
         */
        template <typename U>
            requires std::convertible_to<U, UObject*> && (std::same_as<T, U> || ...)
        explicit TVariantObject(U *Object) : ContainedObject(Object), TypeIndex(GetTypeIndex<U>()) {
            check(Object != nullptr)
        }

        /**
         * Construct a new object from the given interface
         * @tparam U The type of interface to construct from
         * @param Object The interface to construct from
         */
        template <typename U>
            requires UnrealInterface<U> && (std::same_as<T, U> || ...)
        explicit TVariantObject(const TScriptInterface<U> &Object)
            : ContainedObject(Object.GetObject()), TypeIndex(GetTypeIndex<U>()) {
            check(Object != nullptr)
        }

        /**
         * Attempt to construct a new object from the given object. will raise a fatal error if the wrong type.
         * @param Object The object to construct from
         */
        explicit TVariantObject(UObject *Object) : ContainedObject(Object), TypeIndex(GetTypeIndex(Object).GetValue()) {
        }

        /**
         * Attempt to construct a new object from the given interface, will raise a fatal error if the wrong type.
         * @param Object The inteface to construct from
         */
        explicit TVariantObject(const FScriptInterface &Object)
            : ContainedObject(Object.GetObject()), TypeIndex(GetTypeIndex(Object.GetObject()).GetValue()) {
        }

        /**
         * Access any of the members on UObject, regardless of the underlying type.
         * @return The underlying object
         */
        UObject *operator->() const {
            return ContainedObject;
        }

        /**
         * Dereference the underlying UObject, regardless of the underlying type.
         * @return The underlying object
         */
        UObject &operator*() const {
            return *ContainedObject;
        }

        /**
         * Equality operator against a null value.
         * @return Is the variant null?
         */
        constexpr bool operator==(std::nullptr_t) const {
            return IsValid();
        }

        /**
         * Check if the variant is of the given type
         * @tparam U The underlying type to check against
         * @return Is this the given type
         */
        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        constexpr bool IsType() const {
            return TypeIndex == GetTypeIndex<U>();
        }

        /**
         * Runs a check to ensure that the contained state is valid.
         * @return The state is valid
         */
        constexpr bool IsValid() const {
            return !IsType<std::nullptr_t>() && ContainedObject != nullptr;
        }

        /**
         * Applies a given functor to the currently contained value of the variant object,
         * ensuring that the type index is valid and not representing a nullptr. The function
         * dispatches to the appropriate function implementation based on the type index.
         *
         * @param Functor The callable object (functor) that will be applied to the variant's content.
         * @return The result of invoking the functor on the contained object.
         */
        template <typename F>
            requires(std::invocable<F, T *> && ...)
        decltype(auto) Visit(F &&Functor) const {
            check(TypeIndex != GetTypeIndex<std::nullptr_t>())
            static constexpr std::array VisitFunctions = {&TVariantObject::VisitSingle<T, F>...};
            return std::invoke(VisitFunctions[TypeIndex - 1], ContainedObject, std::forward<F>(Functor));
        }

      private:
        template <typename U, typename F>
            requires(std::same_as<T, U> || ...) && std::invocable<F, U *>
        static constexpr decltype(auto) VisitSingle(UObject *Object, F &&Functor) {
            return std::invoke(std::forward<F>(Functor), static_cast<U *>(Object));
        }

      protected:
        /**
         * Get the raw TObjectPtr object, used mainly for linking this type to the garbage collector if needed
         * @return The raw object pointer
         */
        TObjectPtr<UObject> &GetObjectPtr() {
            return ContainedObject;
        }

        /**
         * Get the raw TObjectPtr object, used mainly for linking this type to the garbage collector if needed
         * @return The raw object pointer
         */
        const TObjectPtr<UObject> &GetObjectPtr() const {
            return ContainedObject;
        }

      public:
        /**
         * Get a reference to the held value, will raise a fatal error if the wrong type.
         * @tparam U The type of the value
         * @return A reference to the value
         */
        template <typename U>
            requires(std::same_as<T, U> || ...)
        U &Get() const {
            return *CastChecked<U>(ContainedObject);
        }

        /**
         * Get a reference to the held value, will raise a fatal error if null.
         * @return A reference to the value
         */
        UObject &Get() const {
            check(::IsValid(ContainedObject))
            return *ContainedObject;
        }

        /**
         * Attempt to get an object of the given type.
         * @tparam U The type to check (must be one of the variant types)
         * @return The object, if the cast was successful
         */
        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        TOptional<U &> TryGet() const {
            if (TypeIndex != GetTypeIndex<U>()) {
                return TOptional<U &>();
            }

            return Optionals::OfNullable<TOptional>(static_cast<U *>(ContainedObject));
        }

        /**
         * Try to get the underlying object.
         * @return The underlying object
         */
        TOptional<UObject &> TryGet() const {
            return Optionals::OfNullable<TOptional>(ContainedObject);
        }

        /**
         * Get the type index of the given type.
         * @tparam U The type to check (must be one of the variant types)
         * @return The index of the given type.
         */
        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        static constexpr uint64 GetTypeIndex() {
            constexpr std::array<bool, sizeof...(T) + 1> TypesMatch = {std::same_as<U, std::nullptr_t>,
                                                                       std::same_as<U, T>...};
            auto Find = std::ranges::find_if(TypesMatch, [](bool Matches) { return Matches; });
            check(Find != TypesMatch.end())
            return std::distance(TypesMatch.begin(), Find);
        }

        /**
         * Try to get the type index of the given object
         * @param Object The object to check
         * @return The type index (if found)
         */
        static TOptional<uint64> GetTypeIndex(const UObject *Object) {
            constexpr std::array TypeChecks = {&TVariantObject::IsValidType<std::nullptr_t>,
                                               &TVariantObject::IsValidType<T>...};
            auto Find = std::ranges::find_if(TypeChecks, [Object](auto &&Callback) { return Callback(Object); });
            return Find != TypeChecks.end() ? std::distance(TypeChecks.begin(), Find) : TOptional<uint64>();
        }

        /**
         * Try to get the type index of the given asset
         * @param Data The asset to check
         * @return The type index (if found)
         */
        static TOptional<uint64> GetTypeIndex(const FAssetData &Data) {
            constexpr std::array TypeChecks = {&TVariantObject::IsAssetTypeValid<std::nullptr_t>,
                                               &TVariantObject::IsAssetTypeValid<T>...};
            auto Find = std::ranges::find_if(TypeChecks, [&Data](auto &&Callback) { return std::invoke(Callback, Data); });
            return Find != TypeChecks.end() ? std::distance(TypeChecks.begin(), Find) : TOptional<uint64>();
        }

        static TOptional<uint64> GetTypeIndexForClass(const UClass *Class) {
            constexpr std::array TypeChecks = {&TVariantObject::IsClassValid<std::nullptr_t>,
                                               &TVariantObject::IsClassValid<T>...};
            auto Find = std::ranges::find_if(TypeChecks, [Class](auto &&Callback) { return Callback(Class); });
            return Find != TypeChecks.end() ? std::distance(TypeChecks.begin(), Find) : TOptional<uint64>();
        }

        /**
         * Get the type index of this varaint
         * @return The type index of this varaint
         */
        uint64 GetTypeIndex() const {
            return TypeIndex;
        }

        /**
         * Perform a static (compile-time) check if the given type is valid
         * @tparam U The type to check (must be one of the variant types)
         * @return Is this a valid type
         */
        template <typename U>
        static constexpr bool StaticIsValidType() {
            return (std::same_as<T, U> || ...);
        }

        /**
         * Check if the asset is a valid type (and matches the given type)
         * @tparam U The type to check (must be one of the variant types)
         * @param Data The asset data to check against
         * @return Is this asset of a valid type
         */
        template <typename U>
            requires std::same_as<U, std::nullptr_t> || (std::same_as<T, U> || ...)
        static constexpr bool IsAssetTypeValid(const FAssetData &Data) {
            if constexpr (std::same_as<U, std::nullptr_t>) {
                return !Data.IsValid();
            } else if constexpr (UnrealInterface<U>) {
                return Data.GetClass()->ImplementsInterface(U::UClassType::StaticClass());
            } else {
                return Data.IsInstanceOf<U>();
            }
        }

        template <typename U>
            requires std::same_as<U, std::nullptr_t> || (std::same_as<T, U> || ...)
        static constexpr bool IsClassValid(const UClass *Class) {
            if constexpr (std::same_as<U, std::nullptr_t>) {
                return Class == nullptr;
            } else if constexpr (UnrealInterface<U>) {
                return Class->ImplementsInterface(U::UClassType::StaticClass());
            } else {
                return Class->IsChildOf<U>();
            }
        }

        /**
         * Check if the object is a valid type (and matches the given type)
         * @param Object The object data to check against
         * @return Is this object of a valid type
         */
        static bool IsValidType(const UObject *Object) {
            return GetTypeIndex(Object).IsSet();
        }

        static bool IsValidType(const UClass *Class) {
            if (Class == nullptr) {
                return true;
            }

            static constexpr std::array ValidTypeChecks = {&IsValidSubclass<T>...};
            auto Find = std::ranges::find_if(ValidTypeChecks, [Class](auto &&Callback) { return Callback(Class); });
            return Find != ValidTypeChecks.end();
        }

        /**
         * Get the array of all classes that are usable by this variant type.
         * @return The array of all classes that are usable by this variant type.
         */
        static TArray<UClass *> GetTypeClasses() {
            return {GetClass<T>()...};
        }

        /**
         * Set the value of this variant
         * @tparam U The of the object to set (must be one of the variant types)
         * @param Object The object to set
         */
        template <typename U>
            requires std::is_base_of_v<UObject, U> && (std::same_as<T, U> || ...)
        void Set(U *Object) {
            ContainedObject = Object;
            TypeIndex = GetTypeIndex<U>();
        }

        /**
         * Set the value of this variant
         * @tparam U The of the object to set (must be one of the variant types)
         * @param Object The object to set
         */
        template <typename U>
            requires UnrealInterface<U> && (std::same_as<T, U> || ...)
        void Set(const TScriptInterface<U> &Object) {
            ContainedObject = Object.GetObject();
            TypeIndex = GetTypeIndex<U>();
        }

        /**
         * Set the value of this variant
         * @param Object The object to set
         */
        void Set(UObject *Object) {
            ContainedObject = Object;
            TypeIndex = GetTypeIndex(Object).GetValue();
        }

        /**
         * Set the value of this variant
         * @param Object The object to set
         */
        void Set(const FScriptInterface &Object) {
            ContainedObject = Object.GetObject();
            TypeIndex = GetTypeIndex(Object.GetObject()).GetValue();
        }

      protected:
        /**
         * Perform an unchecked set of a null value (this is used internally for declared variant object structs)
         */
        void SetUnchecked(std::nullptr_t) {
            ContainedObject = nullptr;
            TypeIndex = GetTypeIndex<std::nullptr_t>();
        }

      private:
        explicit TVariantObject(FIntrusiveUnsetOptionalState) : ContainedObject(nullptr) {
        }

        TVariantObject &operator=(FIntrusiveUnsetOptionalState) {
            ContainedObject = nullptr;
            TypeIndex = GetTypeIndex<std::nullptr_t>();
            return *this;
        }

        bool operator==(FIntrusiveUnsetOptionalState) const {
            return !IsValid();
        }

        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        static constexpr bool IsValidType(const UObject *Object) {
            if constexpr (std::is_same_v<std::nullptr_t, U>) {
                return Object == nullptr;
            } else if constexpr (UnrealInterface<U>) {
                return Object->Implements<typename U::UClassType>();
            } else {
                return Object->IsA<U>();
            }
        }

        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        static constexpr UClass *GetUnrealClassType() {
            if constexpr (std::is_same_v<std::nullptr_t, U>) {
                return nullptr;
            } else if constexpr (UnrealInterface<U>) {
                return U::UClassType::StaticClass();
            } else {
                return U::StaticClass();
            }
        }

        friend struct TOptional<TVariantObject>;

        template <typename U>
            requires VariantObjectStruct<U>
        friend class TVariantObjectCustomization;

        TObjectPtr<UObject> ContainedObject;
        uint64 TypeIndex = GetTypeIndex<std::nullptr_t>();
    };
}
#endif