// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UniquePokeRegistry.h"

/**
 * The generic version of the static registry that requires no additional specialized methods, and can act as a
 * singleton object all on its own
 */
template <typename T, typename... Args>
class TGenericStaticUniqueRegistry : public TUniquePokeRegistry<T, Args...> {
    TGenericStaticUniqueRegistry() = default;
    ~TGenericStaticUniqueRegistry() = default;

  public:
    /**
     * Get the singleton instance of the class
     * @return A reference to the only instance of this class
     */
    static TGenericStaticUniqueRegistry &GetInstance() {
        static TGenericStaticUniqueRegistry Instance;
        return Instance;
    }
};
