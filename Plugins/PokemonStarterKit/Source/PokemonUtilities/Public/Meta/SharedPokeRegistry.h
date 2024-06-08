// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

/**
 * Generic registry that generates TSharedRef objects
 * @tparam T The superclass type of the registry
 * @tparam Args The constructor arguments taken in
 */
template <typename T, typename... Args>
class TSharedPokeRegistry : public TPokeRegistry<TSharedRef<T>, Args...> {
  public:
    /**
     * Register the given class for the given key using the default constructor
     * @tparam Derived The subclass to register
     * @param Key The key to use for the registry
     */
    template <typename Derived>
        requires std::is_base_of_v<T, Derived>
    void RegisterClass(FName Key) {
        TPokeRegistry<TSharedRef<T>, Args...>::RegisterFactory(Key, ConstructDerived<Derived>);
    }

  private:
    /**
     * Construct the derived class
     * @tparam Derived The subclass to construct
     * @param Arguments The constructor arguments to pass
     * @return The created instance
     */
    template <typename Derived>
        requires std::is_base_of_v<T, Derived>
    static TSharedRef<T> ConstructDerived(Args... Arguments) {
        return MakeShared<Derived>(Arguments...);
    }
};
