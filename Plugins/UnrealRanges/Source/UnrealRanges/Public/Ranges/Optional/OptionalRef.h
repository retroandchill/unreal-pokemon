// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Pointers.h"
#include "Ranges/RangeConcepts.h"
#include "Templates/NonNullSubclassOf.h"

/**
 * Template specialization for an optional that takes in a reference.
 * @tparam T The type of the optional that was consumed.
 */
template <typename T>
struct TOptional<T &> {

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
    constexpr explicit(false) TOptional(T &Value) : Data(&Value) {
    }

    /**
     * Construct a new optional from a nullable pointer.
     * @param Value
     */
    constexpr explicit(false) TOptional(T *Value) : Data(Value) {
    }

    /**
     * Constructor from nullptr
     */
    constexpr explicit(false) TOptional(nullptr_t) {
    }

    /**
     * Assignment operator from the underlying value.
     * @param Value The value to assign in.
     * @return A reference to this object
     */
    TOptional &operator=(T &Value) {
        Data = &Value;
        return *this;
    }

    /**
     * Emplace a new reference value into the optional.
     * @param Value The value to emplace
     * @return A reference to the emplaced value
     */
    T &Emplace(T &Value) {
        Data = Value;
        return *Data;
    }

    /**
     * Get the value of the optional.
     * @param DefaultValue The default to substitute for an empty optional.
     * @return The retrieved value
     */
    T &Get(T &DefaultValue) const
        requires(!std::is_const_v<T>)
    {
        return Data != nullptr ? *Data : DefaultValue;
    }

    /**
     * Get the value of the optional.
     * @param DefaultValue The default to substitute for an empty optional.
     * @return The retrieved value
     */
    const T &Get(const T &DefaultValue) const {
        return Data != nullptr ? *Data : DefaultValue;
    }

    /**
     * Get a nullable pointer for the optional.
     * @return The retrieved value
     */
    T *GetPtrOrNull() const {
        return Data;
    }

    /**
     * Get a reference to the underlying data.
     * @return The retrieved value
     */
    T &GetValue() const {
        check(Data != nullptr)
        return *Data;
    }

    auto GetValueOptional() const {
        using ResultType = std::remove_const_t<T>;
        return Data != nullptr ? TOptional<ResultType>(*Data) : TOptional<ResultType>();
    }

    /**
     * Convert this value to an Unreal Interface type
     * @return The created script interface
     */
    TScriptInterface<T> GetInterface() const
        requires UE::Ranges::UnrealInterface<T>
    {
        return TScriptInterface<T>(Data != nullptr ? Data->_getUObject() : nullptr);
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
    T &operator*() {
        check(Data != nullptr)
        return *Data;
    }

    /**
     * Dereference operator.
     * @return A reference to the underlying data.
     */
    T &operator*() const {
        check(Data != nullptr)
        return *Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T *operator->() {
        check(Data != nullptr)
        return Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T *operator->() const {
        check(Data != nullptr)
        return Data;
    }

  private:
    T *Data = nullptr;
};

/**
 * Template specialization for TOptional to handle values of void* and const void* since you can't have a reference to
 * void.
 * @tparam V The void type, either void or const void.
 */
template <typename V>
    requires std::same_as<std::remove_cv_t<V>, void>
struct TOptional<V> {

    /**
      * Typedef for the element type used by this container.
      */
    using ElementType = V;

    /**
     * Default constructor, creates an empty optional.
     */
    constexpr TOptional() = default;

    /**
     * Constructor from a void* value.
     *
     * @param Value The value for the optional
     */
    constexpr explicit(false) TOptional(V *Value) : Data(Value) {}

    /**
     * Constructor from nullptr, creates an empty optional.
     */
    constexpr explicit(false) TOptional(nullptr_t) {}

    /**
     * Assignment operator from a void* value.
     * 
     * @param Value The value in question
     * @return A reference to this
     */
    TOptional &operator=(V *Value) {
        Data = Value;
        return *this;
    }

    /**
     * Assignment operator from a nullptr value. Sets the optional to empty.
     * 
     * @return A reference to this
     */
    TOptional &operator=(nullptr_t) {
        Data = nullptr;
        return *this;
    }

    /**
     * Get the value for this optional.
     * 
     * @param Alternate The alternative if this optional is empty.
     * @return The value
     */
    V* Get(V* Alternate) const {
        return Data != nullptr ? Data : Alternate;
    }

    /**
     * Get the value of this optional or nullptr.
     * 
     * @return The value of this optional or nullptr.
     */
    V *GetPtrOrNull() const {
        return Data;
    }

    /**
     * Get the value of this optional, asserting that its set.
     * 
     * @return The value of this optional.
     */
    V* GetValue() const {
        check(IsSet())
        return Data;
    }

    /**
     * Returns if the value is set
     * 
     * @return Is there a valid optional?
     */
    bool IsSet() const {
        return Data != nullptr;
    }

    /**
     * Emplace a new pointer value into the optional.
     * 
     * @param Value The value to emplace
     * @return A reference to the emplaced value
     */
    void Emplace(V* Value) {
        Data = Value;
    }

    /**
     * Reset the data to empty.
     */
    void Reset() {
        Data = nullptr;
    }

    
private:
    V* Data = nullptr;
};

namespace UE::Optionals {

    /**
     * Converts a pointer to an optional type.
     *
     * This function checks if the type T is the same as void (ignoring const/volatile qualifiers).
     * If so, it returns an optional containing the original pointer. Otherwise, it returns an
     * optional containing a reference to the type pointed to by the pointer.
     *
     * @param Ptr A pointer to the object of type T.
     * @return TOptional<T> if T is void, or TOptional<T &> if T is not void.
     */
    template <typename T>
    constexpr auto OfNullable(T *Ptr) {
        if constexpr (std::same_as<std::remove_cv_t<T>, void>) {
            return TOptional<T>(Ptr);
        } else {
            return TOptional<T &>(Ptr);
        }
    }

    /**
     * Creates an optional from a possibly nullable pointer.
     *
     * This function takes a pointer of type T, extracts the raw pointer,
     * and returns an optional containing it.
     *
     * @param Ptr A constant reference to a pointer of type T.
     * @return An optional containing the raw pointer extracted from Ptr.
     */
    template <typename T>
        requires Ranges::Pointer<T> && (!std::is_pointer_v<T>)
    constexpr auto OfNullable(const T &Ptr) {
        return OfNullable(Ranges::GetRawPointer<T>(Ptr));
    }

    /**
     * Creates a TOptional containing a TNonNullSubclassOf object from a given TSubclassOf pointer.
     *
     * @param Ptr The TSubclassOf pointer that may be null.
     * @return TOptional<TNonNullSubclassOf<T>> containing the given pointer if it is non-null.
     */
    template <typename T>
    constexpr TOptional<TNonNullSubclassOf<T>> OfNullable(TSubclassOf<T> Ptr) {
        return TOptional<TNonNullSubclassOf<T>>(Ptr);
    }

} // namespace UE::Optionals
