// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonCoroutineDispatcher.h"
#include "Kismet/GameplayStatics.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"

DEFINE_INJECTABLE_DEPENDENCY(IPokemonCoroutineDispatcher)

IPokemonCoroutineDispatcher::FStorage IPokemonCoroutineDispatcher::Instance;

bool IPokemonCoroutineDispatcher::Init(UObject* Outer) {
    if (Instance.Interface != nullptr) {
        return false;
    }

    Instance.Interface = UnrealInjector::NewInjectedDependency<IPokemonCoroutineDispatcher>(Outer);
    return true;
}

bool IPokemonCoroutineDispatcher::Destroy() {
    if (Instance.Interface == nullptr) {
        return false;
    }

    Instance.Interface = nullptr;
    return true;
}

void IPokemonCoroutineDispatcher::FStorage::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Interface.GetObjectRef());
}

FString IPokemonCoroutineDispatcher::FStorage::GetReferencerName() const {
    return "IPokemonCoroutineDispatcher::FStorage";
}