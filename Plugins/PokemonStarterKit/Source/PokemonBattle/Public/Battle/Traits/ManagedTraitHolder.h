// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include <array>

/**
 * String literal representation that holds an array of characters
 * @tparam N The size of the character array
 */
template <auto N>
struct TStringLiteral {
    constexpr explicit(false) TStringLiteral(const TCHAR (&Str)[N]) {
        std::copy_n(Str, N, Value.data());
    }

    std::array<TCHAR, N> Value;
};

/**
 * Managed type for the data asset that hold trait information of some kind
 * @tparam T The type of asset to load
 * @tparam Path The path that the assets are loaded from
 */
template <typename T, TStringLiteral Path>
class TManagedTraitHolder {
public:
    /**
     * Constructs an empty manager with no ID assigned
     */
    TManagedTraitHolder() = default;

    /**
     * Constructs a new instance with the given ID
     * @param IdIn The ID to assign
     */
    explicit TManagedTraitHolder(FName IdIn) : ID(IdIn) {
        
    }

    /**
     * Attempt to get the object, lazily loading if it has not been set
     * @return The object that this holder contains.
     */
    T* Get() const {
        if (bLoaded) {
            return Object.Get();
        }

        auto Format = FString::Format(TEXT("{0}/{1}.{1}"), { Path.Value.data(), ID.ToString() });
        Object.Reset(Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Format)));
        bLoaded = true;
        return Object.Get();
    }

    /**
     * Dereference operator for the managed resource
     * @return The object that this holder contains.
     */
    T& operator*() const {
        return *Get();
    }

    /**
     * Pointer member access operator for the managed resource
     * @return The object that this holder contains.
     */
    T* operator->() const {
        return Get();
    }

    /**
     * Set the ID to the new ID, while also invalidating the current lazy loading
     * @param NewID The new ID of the managed object
     */
    void SetID(FName NewID) {
        ID = NewID;
        Object.Reset();
        bLoaded = false;
    }

private:
    /**
     * The current ID of the loaded asset
     */
    FName ID;
    
    /**
     * The actual managed object
     */
    mutable TStrongObjectPtr<T> Object;

    /**
     * Has there been an attempt to load the object yet
     */
    mutable bool bLoaded = false;
};
