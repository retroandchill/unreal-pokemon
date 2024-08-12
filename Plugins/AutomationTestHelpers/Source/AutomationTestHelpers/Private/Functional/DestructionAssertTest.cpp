// "Unreal Pokémon" created by Retro & Chill.


#include "Functional/DestructionAssertTest.h"


void ADestructionAssertTest::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    FinalAssert(EndPlayReason);
    Super::EndPlay(EndPlayReason);
}