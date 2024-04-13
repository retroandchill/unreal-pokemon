// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

/**
 * Signifies a generic static registry for different data types. Used to create a static registry that sits below
 * Unreal Engine and doesn't require the classes to be UObjects
 * @tparam Ptr The pointer type that gets created
 * @tparam Args The constructor arguments taken in
 */
template <typename Ptr, typename... Args>
class TPokeRegistry {
  public:
    /**
     * Factory function that produces a unique pointer to referenced type
     */
    using FFactoryFunction = TFunction<Ptr(Args...)>;

    /**
     * Register the given method as a factory function for the given key
     * @param Key The key to use for the registry
     * @param FactoryFunction
     */
    void RegisterFactory(FName Key, const FFactoryFunction &FactoryFunction) {
        RegisteredConstructors.Add(Key, FactoryFunction);
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
    TArray<FName> GetAllRegisteredTypes() const {
        TArray<FName> Keys;
        RegisteredConstructors.GetKeys(Keys);
        return Keys;
    }

  private:
    /**
     * The set of registered constructors for the data table proxy types
     */
    TMap<FName, FFactoryFunction> RegisteredConstructors;
};
