// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PokemonCoroutineDispatcher.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"

DEFINE_INJECTABLE_DEPENDENCY(IPokemonCoroutineDispatcher)

IPokemonCoroutineDispatcher &IPokemonCoroutineDispatcher::Get(const UObject *WorldContext) {
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    check(GameInstance != nullptr)
    auto Subsystem = GameInstance->GetSubsystem<UPokemonSubsystem>();
    check(Subsystem != nullptr)
    return Subsystem->GetCoroutineDispatcher();
}