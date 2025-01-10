// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Ranges/Views/Generator.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Meta {
    /**
     * Signifies a generic static registry for different data types. Used to create a static registry that sits below
     * Unreal Engine and doesn't require the classes to be UObjects
     * @tparam T The type contained within the pointer
     * @tparam Args The constructor arguments taken in
     */
    RETROLIB_EXPORT template <Class T, bool bShared, typename... Args>
    class TRegistry {
      public:
        using Ptr = std::conditional_t<bShared, TSharedRef<T>, TUniquePtr<T>>;
        using FIdentifier = FName;
        /**
         * Factory function that produces a unique pointer to referenced type
         */
        using FFactoryFunction = TFunction<Ptr(Args...)>;

        /**
         * Register the given method as a factory function for the given key
         * @param Key The key to use for the registry
         * @param FactoryFunction
         */
        void RegisterFactory(FName Key, FFactoryFunction FactoryFunction) {
            RegisteredConstructors.Add(Key, std::move(FactoryFunction));
        }

        /**
         * Register the given class for the given key using the default constructor
         * @tparam Derived The subclass to register
         * @param Key The key to use for the registry
         */
        template <typename Derived>
            requires std::derived_from<Derived, T>
        void RegisterClass(FName Key) {
            RegisterFactory(Key, ConstructDerived<Derived>);
        }

        /**
         * Construct an instance of the given class
         * @param Key The key to look up the constructor for
         * @param Arguments The arguments to pass to the factory function to
         * @return A unique reference to the factory instance
         */
        Ptr Construct(FName Key, Args... Arguments) const {
            check(RegisteredConstructors.Contains(Key))
            return RegisteredConstructors[Key](Arguments...);
        }

        /**
         * Get if the type in question is registered in as a valid constructor
         * @param Key The for the type in question
         * @return Is the type registered?
         */
        bool IsTypeRegistered(FName Key) const {
            return RegisteredConstructors.Contains(Key);
        }

        /**
         * Get the list of all registered types for this registry.
         * @return The list of all formally registered types.
         */
        TGenerator<FName> GetAllRegisteredTypes() const {
            for (auto &[Key, Func] : RegisteredConstructors) {
                co_yield Key;
            }
        }

      private:
        /**
         * Construct the derived class
         * @tparam Derived The subclass to construct
         * @param Arguments The constructor arguments to pass
         * @return The created instance
         */
        template <typename Derived>
            requires std::derived_from<Derived, T>
        static Ptr ConstructDerived(Args... Arguments) {
            if constexpr (bShared) {
                return MakeShared<Derived>(Arguments...);
            } else {
                return MakeUnique<Derived>(Arguments...);
            }
        }

        /**
         * The set of registered constructors for the data table proxy types
         */
        TMap<FName, FFactoryFunction> RegisteredConstructors;
    };

    RETROLIB_EXPORT template <Class T, typename... A>
    using TSharedRegistry = TRegistry<T, true, A...>;

    RETROLIB_EXPORT template <Class T, typename... A>
    using TUniqueRegistry = TRegistry<T, false, A...>;
} // namespace Retro::Meta
#endif