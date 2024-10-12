// "Unreal Pokémon" created by Retro & Chill.

#include "Lookup/InjectionTarget.h"
#include "Lookup/InjectionUtilities.h"

FInjectionTarget::FInjectionTarget() = default;

FInjectionTarget::FInjectionTarget(const UClass *InterfaceType)
    : TargetInterface(InterfaceType),
      InjectedClass(UnrealInjector::GetFirstInjectableObject(InterfaceType).GetPtrOrNull()) {
}