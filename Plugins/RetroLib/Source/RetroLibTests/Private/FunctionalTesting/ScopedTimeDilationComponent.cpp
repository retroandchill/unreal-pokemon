// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTesting/ScopedTimeDilationComponent.h"
#include "FunctionalTesting/ScopedFunctionalTest.h"
#include "RetroLib.h"

void UScopedTimeDilationComponent::AddScopedComponent(AScopedFunctionalTest &TestObject) {
    TestObject.AddScopedData<Retro::FScopedTimeDilationFactor>(this, TimeDilationFactor);
}