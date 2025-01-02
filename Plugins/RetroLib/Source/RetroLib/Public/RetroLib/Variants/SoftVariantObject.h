// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "VariantObject.h"
#include "RetroLib/Assets/AsyncLoadHandle.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
	template <typename T>
        requires VariantObject<T>
    struct TSoftVariantObject;

    namespace Detail {
        template <typename>
        struct TIsSoftVariantObject : std::false_type {};

        template <typename... T>
        struct TIsSoftVariantObject<TSoftVariantObject<T...>> : std::true_type {};
    } // namespace Detail

    /**
     * Checks if the given type is a soft variant object.
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept SoftVariantObject = Detail::TIsSoftVariantObject<T>::value;

    /**
     * Checks if the given type is a soft variant object type that has a valid UStruct representation..
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept SoftVariantObjectStruct = UEStruct<T> && SoftVariantObject<T>;

    template <typename T>
        requires SoftVariantObjectStruct<T>
    class TSoftVariantObjectCustomization;

    /**
     * Represents a soft (path) reference to an object of one of serveral possible types.
     * @tparam T The variant object that dictates the data for this variant
     */
    RETROLIB_EXPORT template <typename T>
        requires VariantObject<T>
    struct TSoftVariantObject {
        static constexpr bool bHasIntrusiveUnsetOptionalState = true;
        using IntrusiveUnsetOptionalStateType = TSoftVariantObject;

        /**
         * Construct this pointer from the given asset information.
         * @tparam A The types of the forwarding arguments
         * @param Args The arguments to pass
         */
        template <typename... A>
            requires std::constructible_from<TSoftObjectPtr<>, A...>
        explicit TSoftVariantObject(A &&...Args)
            : Ptr(std::forward<A>(Args)...), TypeIndex(T::GetTypeIndex(GetAssetData()).GetValue()) {
        }

        /**
         * Create an instance explicitly from the given object
         * @param Object The object to set from
         */
        explicit TSoftVariantObject(const T &Object)
            : Ptr(&Object.Get()), TypeIndex(T::GetTypeIndex(GetAssetData()).GetValue()) {
        }

        constexpr uint64 GetTypeIndex() const {
            return TypeIndex;
        }

        /**
         * Returns asset name string, leaving off the /package/path part
         * @return the asset name string, leaving off the /package/path part
         */
        FString GetAssetName() const {
            return Ptr.GetAssetName();
        }

        /**
         * Returns /package/path string, leaving off the asset name
         * @return /package/path string, leaving off the asset name
         */
        FString GetLongPackageName() const {
            return Ptr.GetAssetName();
        }

        /**
         * Hash function
         * @return The hash for this type
         */
        uint32 GetPtrTypeHash() const {
            return GetTypeHash(Ptr);
        }

        /**
         * Test if this points to a live UObject
         * @return if this points to a live UObject
         */
        bool IsValid() const {
            return Ptr.IsValid();
        }

        /**
         * Synchronously load (if necessary) and return the asset object represented by this asset ptr, and attempts to
         * cast it to the given type.
         * @tparam U The type to try to cast to
         * @return the asset object represented by this asset ptr
         */
        template <typename U>
            requires(T::template StaticIsValidType<T>())
        TOptional<U &> LoadSynchronous() const {
            if (TypeIndex != T::template GetTypeIndex<U>()) {
                return TOptional<U &>();
            }

            return Optionals::OfNullable(static_cast<U *>(Ptr.LoadSynchronous()));
        }

        /**
         * Synchronously load (if necessary) and return the asset object represented by this asset ptr
         * @return the asset object represented by this asset ptr
         */
        TOptional<T> LoadSynchronous() const {
            auto Result = Ptr.LoadSynchronous();
            if (Result == nullptr || !T::IsValidType(Result)) {
                return TOptional<T>();
            }

            return TOptional<T>(T(Result));
        }

        /**
         * Perform an asynchronous load of the given object.
         * @return The handle for the async load
         */
        TSharedRef<TAsyncLoadHandle<T>> LoadAsync() const {
            return TAsyncLoadHandle<T>::Create(ToSoftObjectPath());
        }

        /**
         * Set the value from the given hard reference
         * @param Object Hard reference to set from
         */
        void Set(const T &Object) {
            Ptr = Object.TryGet().GetPtrOrNull();
            TypeIndex = Object.GetTypeIndex();
        }

        /**
         * Set the value from the given soft pointer
         * @param Object The soft object to set from
         */
        void Set(const TSoftObjectPtr<> &Object) {
            Ptr = Object;
            TypeIndex = T::GetTypeIndex(GetAssetData()).GetValue();
        }

        /**
         * Returns the SoftObjectPath that is wrapped by this TSoftVariantObject
         * @return The SoftObjectPath that is wrapped by this TSoftVariantObject
         */
        const FSoftObjectPath &ToSoftObjectPath() const {
            return Ptr.ToSoftObjectPath();
        }

        /**
         * Returns the SoftObjectPtr that is wrapped by this TSoftVariantObject
         * @return The SoftObjectPtr that is wrapped by this TSoftVariantObject
         */
        const TSoftObjectPtr<> &ToSoftObjectPtr() const {
            return Ptr;
        }

        /**
         * Returns string representation of reference, in form /package/path.assetname
         * @return The string representation of reference, in form /package/path.assetname
         */
        FString ToString() const {
            return Ptr.ToString();
        }

        /**
         * Equality operator against the unset optional state
         * @return Is this an unset value
         */
        bool operator==(FIntrusiveUnsetOptionalState) const {
            return Ptr.IsNull();
        }

        /**
         * Get the type index of the given soft object pointer. This is obtained by getting the asset metadata.
         * @param Object The object to check against
         * @return The type index of the asset
         */
        static TOptional<uint64> GetTypeIndex(const TSoftObjectPtr<> &Object) {
            const auto &AssetManager = UAssetManager::Get();
            FAssetData Data;
            AssetManager.GetAssetDataForPath(Object.ToSoftObjectPath(), Data);
            return T::GetTypeIndex(Data);
        }

      private:
        friend struct TOptional<TSoftVariantObject>;

        template <typename U>
            requires SoftVariantObjectStruct<U>
        friend class TSoftVariantObjectCustomization;

        explicit TSoftVariantObject(FIntrusiveUnsetOptionalState) {
        }

        TSoftVariantObject &operator=(FIntrusiveUnsetOptionalState) {
            Ptr.Reset();
            return *this;
        }

        FAssetData GetAssetData() const {
            auto &AssetManager = UAssetManager::Get();
            FAssetData Data;
            AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data);
            return Data;
        }

        TSoftObjectPtr<> Ptr;
        uint64 TypeIndex = T::template GetTypeIndex<std::nullptr_t>();
    };
}
#endif