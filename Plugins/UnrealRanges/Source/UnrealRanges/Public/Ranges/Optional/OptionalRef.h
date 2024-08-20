// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Template specialization for an optional that takes in a reference.
 * @tparam T The type of the optional that was consumed.
 */
template <typename T>
struct TOptional<T&> {

    /**
     * Typedef for the element type used by this container.
     */
    using ElementType = T;

    /**
     * Default constructor, creates an empty optional.
     */
    constexpr TOptional() = default;

    /**
     * Construct a new optional from a reference.
     * @param Value The value to reference in this optional.
     */
    constexpr explicit(false) TOptional(T& Value) : Data(&Value) {
    }

    /**
     * Construct a new optional from a nullable pointer.
     * @param Value 
     */
    constexpr explicit(false) TOptional(T* Value) : Data(Value) {
    }

    constexpr explicit(false) TOptional(nullptr_t) {}

    /**
     * Assignment operator from the underlying value.
     * @param Value The value to assign in.
     * @return A reference to this object
     */
    TOptional& operator=(T& Value) {
        Data = &Value;
        return *this;
    }

    /**
     * Emplace a new reference value into the optional.
     * @param Value The value to emplace
     * @return A reference to the emplaced value
     */
    T& Emplace(T& Value) {
        Data = Value;
        return *Data;
    }

    /**
     * Get the value of the optional.
     * @param DefaultValue The default to substitute for an empty optional.
     * @return The retrieved value
     */
    const T& Get(const T& DefaultValue) const {
        return Data != nullptr ? *Data : DefaultValue;
    }

    /**
     * Get a nullable pointer for the optional.
     * @return The retrieved value
     */
    T* GetPtrOrNull() {
        return Data;
    }

    /**
     * Get a nullable pointer for the optional.
     * @return The retrieved value
     */
    const T* GetPtrOrNull() const {
        return Data;
    }

    /**
     * Get a reference to the underlying data.
     * @return The retrieved value
     */
    T& GetValue() {
        check(Data != nullptr)
        return *Data;
    }

    /**
     * Get a reference to the underlying data.
     * @return The retrieved value
     */
    const T& GetValue() const {
        check(Data != nullptr)
        return *Data;
    }

    auto GetValueOptional() const {
        using ResultType = std::remove_const_t<T>;
        return Data != nullptr ? TOptional<ResultType>(*Data) : TOptional<ResultType>();
    }

    /**
     * Returns if the value is set
     * @return Is there a valid optional?
     */
    bool IsSet() const {
        return Data != nullptr;
    }

    /**
     * Reset the data to empty.
     */
    void Reset() {
        Data = nullptr;
    }

    /**
     * Dereference operator.
     * @return A reference to the underlying data.
     */
    T& operator*() {
        check(Data != nullptr)
        return *Data;
    }

    /**
     * Dereference operator.
     * @return A reference to the underlying data.
     */
    T& operator*() const {
        check(Data != nullptr)
        return *Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T* operator->() {
        check(Data != nullptr)
        return Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T* operator->() const {
        check(Data != nullptr)
        return Data;
    }

private:
    T* Data = nullptr;
};

namespace UE::Optionals {

    template <typename T>
    FORCEINLINE TOptional<T&> OfNullable(T* Ptr) {
        return TOptional<T&>(Ptr);
    }
    
}
