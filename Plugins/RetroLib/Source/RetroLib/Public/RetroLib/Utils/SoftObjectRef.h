// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "Engine/AssetManager.h"
#endif

#include "RetroLib/Assets/AsyncLoadHandle.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

/**
 * Wrapper around TSoftObjectPtr that is (almost always) guaranteed to point to a valid asset
 * @tparam T The type of object that is pointed to
 */
RETROLIB_EXPORT template <typename T = UObject>
	requires std::is_base_of_v<UObject, T>
struct TSoftObjectRef {
	static constexpr bool bHasIntrusiveUnsetOptionalState = true;
	using IntrusiveUnsetOptionalStateType = TSoftObjectRef;

	template <typename... A>
		requires std::constructible_from<TSoftObjectPtr<T>, A...>
	explicit TSoftObjectRef(A&&... Args) : Ptr(std::forward<A>(Args)...) {
		check(IsAssetValid())
	}

	/**
	 * Returns asset name string, leaving off the /package/path part
	 * @return the asset name string, leaving off the /package/path part
	 */
	FString
	GetAssetName() const {
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
	T& LoadSynchronous() const {
		auto Result = Ptr.LoadSynchronous();
		check(::IsValid(Result))
		return *Result;
	}

	TSharedRef<Retro::TAsyncLoadHandle<T>> LoadAsync() const {
		return Retro::TAsyncLoadHandle<T>::Create(ToSoftObjectPath());
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

	bool operator==(FIntrusiveUnsetOptionalState) const {
		return Ptr.IsNull();
	}

	bool IsAssetValid() const {
		const auto& AssetManager = UAssetManager::Get();
		FAssetData Data;
		return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf<T>();
	}

	bool IsAssetOfType(const UClass* AssetType) const {
		const auto& AssetManager = UAssetManager::Get();
		FAssetData Data;
		return AssetManager.GetAssetDataForPath(ToSoftObjectPath(), Data) && Data.IsInstanceOf(AssetType);
	}

private:
	friend struct TOptional<TSoftObjectRef>;

	explicit TSoftObjectRef(FIntrusiveUnsetOptionalState) {
	}

	TSoftObjectRef& operator=(FIntrusiveUnsetOptionalState) {
		Ptr.Reset();
		return *this;
	}

	TSoftObjectPtr<T> Ptr;
};

namespace Retro::Optionals {
	template <typename T>
		requires std::derived_from<T, UObject>
	struct TNullableOptionalParam<TSoftObjectPtr<T>> : FValidType {
		using RawType = TSoftObjectPtr<T>;
		using ReferenceType = TSoftObjectRef<T>;

		template <template <typename...> typename O, typename U>
			requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>>>
		static constexpr auto OfNullable(const TSoftObjectPtr<U> &Ptr) {
			return Ptr.IsNull() ? O<ReferenceType>(Ptr) : O<ReferenceType>();
		}

		template <template <typename...> typename O, typename U>
			requires std::derived_from<U, T> && OptionalType<O<TSoftObjectRef<U>>>
		static constexpr auto OfNullable(TSoftObjectPtr<U> &&Ptr) {
			return Ptr.IsNull() ? O<ReferenceType>(std::move(Ptr)) : O<ReferenceType>();
		}
	};

	template <typename T>
		requires SpecializationOf<std::decay_t<T>, TSoftObjectPtr> && (!std::same_as<std::decay_t<T>, T>)
	struct TNullableOptionalParam<T> : TNullableOptionalParam<std::decay_t<T>> {};
} // namespace UE::Optionals
#endif