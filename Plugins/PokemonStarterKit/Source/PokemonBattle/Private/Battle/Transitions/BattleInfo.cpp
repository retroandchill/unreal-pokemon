// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/BattleInfo.h"
#include "Battle/BattleSide.h"
#include "Managers/PokemonSubsystem.h"

TScriptInterface<IBattleSide> FBattleInfo::CreatePlayerSide(const TScriptInterface<IBattle> &Battle,
                                                            const TSubclassOf<AActor> &SideClass,
                                                            const FTransform &Transform) const {
    auto World = Battle.GetObject()->GetWorld();
    check(World != nullptr)

    auto SideActor = World->SpawnActor(SideClass, &Transform);
    SideActor->AttachToActor(CastChecked<AActor>(Battle.GetObject()),
                             FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    TScriptInterface<IBattleSide> Side = SideActor;
    check(Side.GetInterface() != nullptr)

    return Side->Initialize(Battle, UPokemonSubsystem::GetInstance(World).GetPlayer(), PlayerSideCount, true);
}