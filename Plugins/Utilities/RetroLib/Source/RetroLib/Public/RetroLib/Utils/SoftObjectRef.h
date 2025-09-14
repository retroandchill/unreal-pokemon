// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "AssetRegistry/AssetData.h"
#include "Engine/AssetManager.h"

#if RETROLIB_WITH_UE5CORO
#include "UE5Coro.h"
#endif
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

/**
 * Wrapper around TSoftObjectPtr that is (almost always) guaranteed to point to a valid asset
 * @tparam T The type of object that is pointed to
 */
RETROLIB_EXPORT template <typename T = UObject>
    requires std::is_base_of_v<UObject, T>
struct TSoftObjectRef
{
    static constexpr bool bHasIntrusiveUnsetOptionalState = true;
    using IntrusiveUnsetOptionalStateType = TSoftObjectRef;

    template <typename... A>
        requires std::constructible_from<TSoftObjectPtr<T>, A...> && (!Retro::PackSameAs<TSoftObjectRef, A...>)
    explicit TSoftObjectRef(A &&...Args) : Ptr(std::forward<A>(Args)...)
    {
        check(IsAssetValid());
    }

    /**
     * Returns asset name string, leaving off the /package/path part
     * @return the asset name string, leaving off the /package/path part
     */
    FString GetAssetName() const
    {
        return Ptr.GetAssetName();
    }

    /**
     * Returns /package/path string, leaving off the asset name
     * @return /package/path string, leaving off the asset name
     */
    FString GetLongPackageName() const
    {
        return Ptr.GetAssetName();
    }

    /**
     * Hash function
     * @return The hash for this type
     */
    uint32 GetPtrTypeHash() const
    {
        return Ptr.GetPtrTypeHash();
    }

    /**
     * Test if this points to a live UObject
     * @return if this points to a live UObject
     */
    bool IsValid() const
    {
        return Ptr.IsValid();
    }

    /**
     * Synchronously load (if necessary) and return the asset object represented by this asset ptr
     * @return the asset object represented by this asset ptr
     */
    T &LoadSynchronous() const
    {
        auto Result = Ptr.LoadSynchronous();
        check(::IsValid(Result))
        return *Result;
    }

#if RETROLIB_WITH_UE5CORO
    UE5Coro::TCoroutine<T &> LoadAsync() const
    {
        auto Result = co_await UE5Coro::Latent::AsyncLoadObject(Ptr);
        check(::IsValid(Result))
        co_return *Result;
    }
#endif

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    TSoftObjectPtr<T> &ToSoftObjectPtr() &
    {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const TSoftObjectPtr<T> &ToSoftObjectPtr() const &
    {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    TSoftObjectPtr<T> &&ToSoftObjectPtr() &&
    {
        return std::move(Ptr);
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    operator TSoftObjectPtr<T>() &
    {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    operator TSoftObjectPtr<T>() const &
    {
        return Ptr;
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    operator TSoftObjectPtr<T>() &&
    {
        return std::move(Ptr);
    }

    /**
     * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
     * @return The StringObjectPath that is wrapped by this SoftObjectPtr
     */
    const FSoftObjectPath &ToSoftObjectPath() const
    {
        return Ptr.ToSoftObjectPath();
    }

    /**
     * Returns string representation of reference, in form /package/path.assetname
     * @return The string representation of reference, in form /package/path.assetname
     */
    FString ToString() const
    {
        return Ptr.ToString();
    }

    bool operator==(FIntrusiveUnsetOptionalState) const
    {
        return Ptr.IsNull();
    }

    bool IsAssetValid() const
    {
        const auto &AssetManager = UAssetManager::Get();
        FAssetData Data;
        return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf<T>();
    }

    bool IsAssetOfType(const UClass *AssetType) const
    {
        const auto &AssetManager = UAssetManager::Get();
        FAssetData Data;
        return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf(AssetType);
    }

  private:
    friend struct TOptional<TSoftObjectRef>;

    explicit TSoftObjectRef(FIntrusiveUnsetOptionalState)
    {
    }

    TSoftObjectRef &operator=(FIntrusiveUnsetOptionalState)
    {
        Ptr.Reset();
        return *this;
    }

    TSoftObjectPtr<T> Ptr;
};

namespace Retro::Optionals
{
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject>
    struct TNullableOptionalParam<TSoftObjectPtr<T>> : FValidType
    {
        using RawType = TSoftObjectPtr<T>;
        using ReferenceType = TSoftObjectRef<T>;

        template <template <typename...> typename O, typename U>
            requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>>>
        static constexpr auto OfNullable(const TSoftObjectPtr<U> &Ptr)
        {
            return !Ptr.IsNull() ? O<ReferenceType>(ReferenceType(Ptr)) : O<ReferenceType>();
        } // namespace Retro::Optionals

        template <template <typename...> typename O, typename U>
            requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>>>
        static constexpr auto OfNullable(TSoftObjectPtr<U> &&Ptr)
        {
            return !Ptr.IsNull() ? O<ReferenceType>(ReferenceType(std::move(Ptr))) : O<ReferenceType>();
        }

        template <template <typename...> typename O, typename U, typename E, typename... A>
            requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>, E>>
        static constexpr auto OfNullable(const TSoftObjectPtr<U> &Ptr, A &&...Args)
        {
            return !Ptr.IsNull() ? O<ReferenceType, E>(ReferenceType(Ptr))
                                 : CreateEmptyExpected<O, ReferenceType, E>(std::forward<A>(Args)...);
        }

        template <template <typename...> typename O, typename U, typename E, typename... A>
            requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>, E>>
        static constexpr auto OfNullable(TSoftObjectPtr<U> &&Ptr, A &&...Args)
        {
            return !Ptr.IsNull() ? O<ReferenceType, E>(ReferenceType(std::move(Ptr)))
                                 : CreateEmptyExpected<O, ReferenceType, E>(std::forward<A>(Args)...);
        }
    };

    RETROLIB_EXPORT template <typename T>
        requires SpecializationOf<std::decay_t<T>, TSoftObjectPtr> && (!std::same_as<std::decay_t<T>, T>)
    struct TNullableOptionalParam<T> : TNullableOptionalParam<std::decay_t<T>>
    {
    };
} // namespace Retro::Optionals
#endif