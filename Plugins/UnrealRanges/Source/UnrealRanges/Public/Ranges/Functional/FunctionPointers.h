// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Template class to create a thin wrapper around function pointers. This is mainly here because I don't like the syntax of raw function pointers.
     *
     * @tparam F The type of the function pointer.
     */
    template <typename F>
    class TFunctionPtr;

    /**
     * @class TFunctionPtr
     *
     * @brief This class encapsulates a pointer to a function of a specified type.
     *
     * The TFunctionPtr class is designed to provide type-safe storage and invocation of function pointers.
     * It stores pointers to member functions and allows the bound member function to be called with appropriate arguments.
     *
     * @tparam R The return type of the member function.
     * @tparam A The argument types of the member function.
     */
    template <typename R, typename... A>
    class TFunctionPtr<R(A...)> {
    public:
        /**
         * Alias for the function pointer type itself
         */
        using FunctionPtrType = R (*)(A...);

        /**
         * Default constructor for TFunctionPtr.
         *
         * Constructs a TFunctionPtr with a null function pointer.
         *
         * @return A default-constructed TFunctionPtr with no function assigned.
         */
        constexpr TFunctionPtr() = default;

        /**
         * Constructs a TFunctionPtr with the given function pointer.
         *
         * This constructor initializes the TFunctionPtr by assigning the provided function pointer to it.
         *
         * @param Function The function pointer to be assigned to the TFunctionPtr.
         * @return A TFunctionPtr initialized with the specified function pointer.
         */
        constexpr explicit(false) TFunctionPtr(FunctionPtrType Function) : Function(Function) {}

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr FunctionPtrType Get() const { return Function; }

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr operator FunctionPtrType() const { return Function; }

        /**
         * Assignment operator from the raw pointer type.
         * 
         * @param NewFunction The new value of the function pointer.
         * @return A reference to this object
         */
        constexpr TFunctionPtr& operator=(FunctionPtrType NewFunction) {
            Function = NewFunction;
            return *this;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(const TFunctionPtr &Other) const {
            return Function == Other.Function;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(FunctionPtrType Other) const { return Function == Other; }

        /**
         * Invocation operator for the function pointer.
         * 
         * @param Args The arguments for the call
         * @return The return value of the function call
         */
        constexpr R operator()(A... Args) const {
            return Function(Args...);
        }

    private:
        FunctionPtrType Function = nullptr;
    };

    /**
     * @class TFunctionPtr
     *
     * @brief This class encapsulates a pointer to a function of a specified type.
     *
     * The TFunctionPtr class is designed to provide type-safe storage and invocation of function pointers.
     * It stores pointers to member functions and allows the bound member function to be called with appropriate arguments.
     *
     * @tparam R The return type of the member function.
     * @tparam A The argument types of the member function.
     */
    template <typename R, typename... A>
    class TFunctionPtr<R(A...) noexcept> {
    public:
        /**
         * Alias for the function pointer type itself
         */
        using FunctionPtrType = R (*)(A...) noexcept;

        /**
         * Default constructor for TFunctionPtr.
         *
         * Constructs a TFunctionPtr with a null function pointer.
         *
         * @return A default-constructed TFunctionPtr with no function assigned.
         */
        constexpr TFunctionPtr() = default;

        /**
         * Constructs a TFunctionPtr with the given function pointer.
         *
         * This constructor initializes the TFunctionPtr by assigning the provided function pointer to it.
         *
         * @param Function The function pointer to be assigned to the TFunctionPtr.
         * @return A TFunctionPtr initialized with the specified function pointer.
         */
        constexpr explicit(false) TFunctionPtr(FunctionPtrType Function) : Function(Function) {}

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr FunctionPtrType Get() const { return Function; }

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr operator FunctionPtrType() const { return Function; }

        /**
         * Assignment operator from the raw pointer type.
         * 
         * @param NewFunction The new value of the function pointer.
         * @return A reference to this object
         */
        constexpr TFunctionPtr& operator=(FunctionPtrType NewFunction) {
            Function = NewFunction;
            return *this;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(const TFunctionPtr &Other) const {
            return Function == Other.Function;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(FunctionPtrType Other) const { return Function == Other; }

        /**
         * Invocation operator for the function pointer.
         * 
         * @param Args The arguments for the call
         * @return The return value of the function call
         */
        constexpr R operator()(A... Args) const {
            return Function(Args...);
        }

    private:
        FunctionPtrType Function = nullptr;
    };

    /**
     * @class TFunctionPtr
     *
     * @brief This class encapsulates a pointer to a member function of a specified type.
     *
     * The TFunctionPtr class is designed to provide type-safe storage and invocation of member function pointers.
     * It stores pointers to member functions and allows the bound member function to be called with appropriate arguments.
     *
     * @tparam T The type of the owning object
     * @tparam R The return type of the member function.
     * @tparam A The argument types of the member function.
     */
    template <typename R, typename T, typename... A>
    class TFunctionPtr<R(T::*)(A...)> {
    public:
        /**
         * Alias for the function pointer type itself
         */
        using FunctionPtrType = R (T::*)(A...);

        /**
         * Default constructor for TFunctionPtr.
         *
         * Constructs a TFunctionPtr with a null function pointer.
         *
         * @return A default-constructed TFunctionPtr with no function assigned.
         */
        constexpr TFunctionPtr() = default;

        /**
         * Default constructor for TFunctionPtr.
         *
         * Constructs a TFunctionPtr with a null function pointer.
         *
         * @return A default-constructed TFunctionPtr with no function assigned.
         */
        constexpr explicit(false) TFunctionPtr(FunctionPtrType Function) : Function(Function) {}

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr FunctionPtrType Get() const { return Function; }

        /**
         * Retrieves the raw function pointer value. Needed for functions/classes that want a raw function pointer
         *
         * @return The raw function pointer value
         */
        constexpr operator FunctionPtrType() const { return Function; }

        /**
         * Assignment operator from the raw pointer type.
         * 
         * @param NewFunction The new value of the function pointer.
         * @return A reference to this object
         */
        constexpr TFunctionPtr& operator=(FunctionPtrType NewFunction) {
            Function = NewFunction;
            return *this;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(const TFunctionPtr &Other) const {
            return Function == Other.Function;
        }

        /**
         * Equality operator with another function pointer object.
         * 
         * @param Other The other function pointer
         * @return Are the two function pointers the same
         */
        constexpr bool operator==(FunctionPtrType Other) const { return Function == Other; }

        /**
         * Dereferencing operator used for making an invocation to this member function pointer (e.g. (Obj.*this)(Args...)).
         * 
         * @return The dereferenced function pointer
         */
        constexpr auto operator*() const {
            return *Function;
        }

    private:
        FunctionPtrType Function = nullptr;
    };
}
