// "Unreal Pokémon" created by Retro & Chill.


#include "FunctionalTesting/ScopedTimeDilationComponent.h"
#include "FunctionalTesting/ScopedFunctionalTest.h"
#include "Ranges/Utilities/ScopedTimeDilationFactor.h"

void UScopedTimeDilationComponent::AddScopedComponent(AScopedFunctionalTest& TestObject) {
    TestObject.AddScopedData<UE::Ranges::FScopedTimeDilationFactor>(this, TimeDilationFactor);
}