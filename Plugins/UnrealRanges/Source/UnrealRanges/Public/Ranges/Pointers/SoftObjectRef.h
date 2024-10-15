// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AsyncLoadHandle.h"
#include "Ranges/Optional/OptionalRef.h"

/**
 * Wrapper around TSoftObjectPtr that is (almost always) guaranteed to point to a valid asset
 * @tparam T The type of object that is pointed to
 */
template <typename T = UObject>
    requires std::is_base_of_v<UObject, T>
struct TSoftObjectRef {
    static constexpr bool bHasIntrusiveUnsetOptionalState = true;
    using IntrusiveUnsetOptionalStateType = TSoftObjectRef;

    template <typename... A>
        requires std::constructible_from<TSoftObjectPtr<T>, A...>
    explicit TSoftObjectRef(A &&... Args) : Ptr(Forward<A>(Args)...) {
        check(IsAssetValid())
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
        return Ptr.GetPtrTypeHash();
    }

    /**
     * Test if this points to a live UObject
     * @return if this points to a live UObject
     */
    bool IsValid() const {
        return Ptr.IsValid();
    }

    /**
     * Synchronously load (if necessary) and return the asset object represented by this asset ptr
     * @return the asset object represented by this asset ptr
     */
    T &LoadSynchronous() const {
        auto Result = Ptr.LoadSynchronous();
        check(::IsValid(Result));
        return *Result;
    }

    TSharedRef<UE::Ranges::TAsyncLoadHandle<T>> LoadAsync() const {
        return UE::Ranges::TAsyncLoadHandle<T>::Create(ToSoftObjectPath());
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const TSoftObjectPtr<T> &ToSoftObjectPtr() const {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const FSoftObjectPath &ToSoftObjectPath() const {
        return Ptr.ToSoftObjectPath();
    }

    /**
     * Returns string representation of reference, in form /package/path.assetname
     * @return The string representation of reference, in form /package/path.assetname
     */
    FString ToString() const {
        return Ptr.ToString();
    }

    bool operator==(FIntrusiveUnsetOptionalState) const {
        return Ptr.IsNull();
    }

    bool IsAssetValid() const {
        auto &AssetManager = UAssetManager::Get();
        FAssetData Data;
        return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf<T>();
    }

    bool IsAssetOfType(const UClass *AssetType) const {
        auto &AssetManager = UAssetManager::Get();
        FAssetData Data;
        return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf(AssetType);
    }

private:
    friend struct TOptional<TSoftObjectRef>;

    explicit TSoftObjectRef(FIntrusiveUnsetOptionalState) {
    }

    TSoftObjectRef &operator=(FIntrusiveUnsetOptionalState) {
        Ptr.Reset();
        return *this;
    }

    TSoftObjectPtr<T> Ptr;
};

namespace UE::Optionals {
    template <typename T>
    constexpr TOptional<TSoftObjectRef<T>> OfNullable(const TSoftObjectPtr<T> &Ptr) {
        if (Ptr.IsNull()) {
            return TOptional<TSoftObjectRef<T>>();
        }

        return TOptional<TSoftObjectRef<T>>(TSoftObjectRef<T>(Ptr));
    }

    template <typename T>
    constexpr TOptional<TSoftObjectRef<T>> OfNullable(TSoftObjectPtr<T> &&Ptr) {
        if (Ptr.IsNull()) {
            return TOptional<TSoftObjectRef<T>>();
        }

        return TOptional<TSoftObjectRef<T>>(TSoftObjectRef<T>(MoveTemp(Ptr)));
    }
} // namespace UE::Optionals