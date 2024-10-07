// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Optional/OptionalRef.h"

/**
 * Wrapper around TSoftObjectPtr that is (almost always) guaranteed to point to a valid asset
 * @tparam T The type of object that is pointed to
 */
template <typename T = UObject>
    requires std::is_base_of_v<UObject, T>
struct TSoftObjectRef {
    static constexpr bool bHasIntrusiveUnsetOptionalState = true;
    
    template <typename... A>
        requires std::constructible_from<TSoftObjectPtr<T>, A...>
    explicit TSoftObjectRef(A&&... Args) : Ptr(Forward<A>(Args)...) {
        check(!Ptr.IsNull())
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
    TOptional<T&> LoadSynchronous() {
        return UE::Optionals::OfNullable(Ptr.LoadSynchronous());
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const TSoftObjectPtr<T>& ToSoftObjectPtr() const {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const FSoftObjectPath& ToSoftObjectPath() const {
        return Ptr.ToSoftObjectPath();
    }

    /**
     * Returns string representation of reference, in form /package/path.assetname
     * @return The string representation of reference, in form /package/path.assetname
     */
    FString ToString() const {
        return Ptr.ToString();
    }

private:
    friend class TOptional<TSoftObjectRef>;

    explicit TSoftObjectRef(FIntrusiveUnsetOptionalState) {}

    TSoftObjectRef& operator=(FIntrusiveUnsetOptionalState) {
        Ptr.Reset();
        return *this;
    }
    
    TSoftObjectPtr<T> Ptr;
};

namespace UE::Optionals {
    template <typename T, typename P>
        requires std::is_same_v<std::remove_cvref_t<P>, TSoftObjectPtr<T>>
    constexpr TOptional<TSoftObjectRef<T>> OfNullable(P&& Ptr) {
        if (Ptr.IsNull()) {
            return TOptional<TSoftObjectRef<T>>();
        }
        
        return TOptional<TSoftObjectRef<T>>(Forward<P>(Ptr));
    }
}