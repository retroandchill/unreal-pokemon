// "Unreal Pokémon" created by Retro & Chill.

#include "DependencyInjectionSubsystem.h"
#include "DependencyInjectionSettings.h"

void UDependencyInjectionSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    InjectionSettings = NewObject<UDependencyInjectionSettings>(this);
}