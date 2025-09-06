// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/InitiateBattle.h"
#include "Battle/Transitions/BattleTransitionSubsystem.h"
#include "Managers/PokemonSubsystem.h"

UInitiateBattle *UInitiateBattle::InitiateBattle(const UObject *WorldContextObject, const FBattleInfo &BattleInfo,
                                                 TSubclassOf<ABattleTransitionActor> Transition)
{
    auto Node = NewObject<UInitiateBattle>();
    Node->SetWorldContext(WorldContextObject);
    Node->BattleInfo = BattleInfo;
    Node->Transition = Transition;
    return Node;
}

UE5Coro::TCoroutine<> UInitiateBattle::ExecuteCoroutine(FForceLatentCoroutine)
{
    auto Subsystem = GetWorldContext()->GetWorld()->GetSubsystem<UBattleTransitionSubsystem>();
    check(Subsystem != nullptr)
    if (auto Result = co_await Subsystem->InitiateBattle(std::move(BattleInfo), Transition);
        Result != EBattleResult::Defeat || BattleInfo.bLossAllowed)
    {
        AfterBattle.Broadcast(Result);
    }
    else
    {
        UPokemonSubsystem::GetInstance(GetWorldContext()).PerformPlayerReset();
    }
}
