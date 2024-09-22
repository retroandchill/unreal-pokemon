// "Unreal Pokémon" created by Retro & Chill.

#include "Lookup/InjectionTarget.h"
#include "Lookup/InjectionUtilities.h"

FInjectionTarget::FInjectionTarget() = default;

FInjectionTarget::FInjectionTarget(UClass *InterfaceType)
    : TargetInterface(InterfaceType),
      InjectedClass(InterfaceType->IsChildOf<UInterface>() ? UnrealInjector::GetFirstInjectableObject(InterfaceType)
                                                           : nullptr) {
}