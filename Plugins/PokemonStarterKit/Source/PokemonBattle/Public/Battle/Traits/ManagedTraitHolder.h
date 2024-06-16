﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <array>

/**
 * String literal representation that holds an array of characters
 * @tparam N The size of the character array
 */
template <auto N>
struct TStringLiteral {
    /**
     * Implicit constructor from a string literal
     * @param Str The string to copy the data in from
     */
    constexpr explicit(false) TStringLiteral(const TCHAR (&Str)[N]) {
        std::copy_n(Str, N, Value.data());
    }

    /**
     * Array that holds the characters of the string
     */
    std::array<TCHAR, N> Value;
};

/**
 * Managed type for the data asset that hold trait information of some kind
 * @tparam T The type of asset to load
 * @tparam Path The path that the assets are loaded from
 */
template <typename T, TStringLiteral Path>
class TManagedTraitHolder : public FGCObject {
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
    T *Get() const {
        if (bLoaded) {
            return Object;
        }

        auto Format = FString::Format(TEXT("{0}/{1}.{1}"), {Path.Value.data(), ID.ToString()});
        Object = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Format));
        bLoaded = true;
        return Object;
    }

    /**
     * Dereference operator for the managed resource
     * @return The object that this holder contains.
     */
    T &operator*() const {
        return *Get();
    }

    /**
     * Pointer member access operator for the managed resource
     * @return The object that this holder contains.
     */
    T *operator->() const {
        return Get();
    }

    /**
     * Set the ID to the new ID, while also invalidating the current lazy loading
     * @param NewID The new ID of the managed object
     */
    void SetID(FName NewID) {
        ID = NewID;
        Object = nullptr;
        bLoaded = false;
    }

    void AddReferencedObjects(FReferenceCollector &Collector) override {
        Collector.AddReferencedObject(Object);
    }

    FString GetReferencerName() const override {
        return TEXT("TManagedTraitHolder");
    }

private:
    /**
     * The current ID of the loaded asset
     */
    FName ID;

    /**
     * The actual managed object
     */
    mutable TObjectPtr<T> Object;

    /**
     * Has there been an attempt to load the object yet
     */
    mutable bool bLoaded = false;
};