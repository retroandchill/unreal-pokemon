#include "Asserts.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TrainerHelpers.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestExpGain, "Unit Tests.Battle.Battlers.TestExpGain",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestExpGain::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GLACEON"),
                                                                                 .Level = 75,
                                                                                 .IVs = {{"ATTACK", 31}},
                                                                                 .EVs = {{"ATTACK", 104}},
                                                                                 .Nature = FName("TIMID"),
                                                                                 .Moves = {{.Move = TEXT("ICEFANG")}}});
    auto &PlayerCharacter = UTrainerHelpers::GetPlayerCharacter(World.Get());
    PlayerCharacter->AddPokemonToParty(Pokemon1);

    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GARCHOMP"),
                                                                                 .Level = 65,
                                                                                 .IVs = {{"DEFENSE", 31}},
                                                                                 .EVs = {{"DEFENSE", 92}},
                                                                                 .Nature = FName("JOLLY")});

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, PlayerCharacter, 1, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    auto Result = Battler2->GiveExpToParticipants();
    UE_CHECK_EQUAL(1, Result.Num());
    UE_CHECK_TRUE(Result[0].GainingBattler == Battler1);
    UE_CHECK_EQUAL(1642, Result[0].Amount);

    Battler2->RecordParticipation();
    Result = Battler2->GiveExpToParticipants();
    UE_CHECK_EQUAL(1, Result.Num());
    UE_CHECK_TRUE(Result[0].GainingBattler == Battler1);
    UE_CHECK_EQUAL(3283, Result[0].Amount);

    Battler1->TakeBattleDamage(static_cast<int32>(Battler1->GetAbilityComponent()->GetCoreAttributes()->GetHP()));
    UE_CHECK_TRUE(Battler1->IsFainted());
    Result = Battler2->GiveExpToParticipants();
    UE_CHECK_EQUAL(1, Result.Num());
    UE_CHECK_TRUE(Result[0].GainingBattler == Battler1);
    UE_CHECK_EQUAL(0, Result[0].Amount);

    return true;
}