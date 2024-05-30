// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceInjection.h"

/**
 * 
 */
class UNREALINJECTOR_API FInjectionRegistry {
    FInjectionRegistry();
    FInjectionRegistry(const FInjectionRegistry&) = delete;
    FInjectionRegistry(FInjectionRegistry&&) = delete;
    ~FInjectionRegistry();

    FInjectionRegistry& operator=(const FInjectionRegistry&) = delete;
    FInjectionRegistry& operator=(FInjectionRegistry&&) = delete;

public:
    static FInjectionRegistry& GetInstance();

    template <typename T>
    requires std::is_base_of_v<UInterface, T>
    bool RegisterInjection() {
        Injections.Add(MakeUnique<TInterfaceInjection<T>>());
        return true;
    }

    void ForEachInjection(TFunction<void(const IRegisteredInjection&)> Callback);

    template <typename T>
    static UClass* GetClassForInterface();

private:
    TArray<TUniquePtr<IRegisteredInjection>> Injections;
};

#define DECLARE_INJECTABLE_INTERFACE(InterfaceName) \
    template <> \
    inline UClass* FInjectionRegistry::GetClassForInterface<I##InterfaceName>() { \
        return U##InterfaceName::StaticClass(); \
    }

#define IMPLEMENT_INJECTABLE_INTERFACE(InterfaceName) \
    const bool GRegistered##InterfaceName = FInjectionRegistry::GetInstance().RegisterInjection<U##InterfaceName>();