// "Unreal Pokémon" created by Retro & Chill.


#include "Meta/InjectionRegistry.h"


FInjectionRegistry::FInjectionRegistry() = default;

FInjectionRegistry::~FInjectionRegistry() = default;

FInjectionRegistry & FInjectionRegistry::GetInstance() {
    static FInjectionRegistry Registry;
    return Registry;
}

void FInjectionRegistry::ForEachInjection(TFunction<void(const IRegisteredInjection &)> Callback) {
    for (const auto& Injection : Injections) {
        Callback(*Injection);
    }
}