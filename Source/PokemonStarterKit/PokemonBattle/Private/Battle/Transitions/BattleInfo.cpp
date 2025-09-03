// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleInfo.h"
#include "Battle/BattleSide.h"
#include "Managers/PokemonSubsystem.h"

UE5Coro::TCoroutine<TScriptInterface<IBattleSide>> FBattleInfo::CreatePlayerSide(TScriptInterface<IBattle> Battle,
                                                                                 TSubclassOf<AActor> SideClass,
                                                                                 const FTransform &Transform,
                                                                                 FForceLatentCoroutine) const {
    auto World = Battle.GetObject()->GetWorld();
    check(World != nullptr)

    auto SideActor = World->SpawnActor(SideClass, &Transform);
    SideActor->AttachToActor(CastChecked<AActor>(Battle.GetObject()),
                             FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    TScriptInterface<IBattleSide> Side = SideActor;
    check(Side.GetInterface() != nullptr)

    return Side->Initialize(Battle, UPokemonSubsystem::GetInstance(World).GetPlayer(), PlayerSideCount, true);
}