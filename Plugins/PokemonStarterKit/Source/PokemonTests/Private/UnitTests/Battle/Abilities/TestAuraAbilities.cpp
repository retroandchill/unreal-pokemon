#include "Asserts.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Battle/Traits/Damage/DamageModificationTrait.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAuraAbilities, "Unit Tests.Battle.Abilities.TestAuraAbilities",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAuraAbilities::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto PokemonSubsystem = GameInstance->GetSubsystem<UPokemonSubsystem>();
    PokemonSubsystem->StartNewGame();
    auto PlayerPokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("XERNEAS"), .Level = 50, .Ability = FName(TEXT("FAIRYAURA"))});
    PokemonSubsystem->GetPlayer()->AddPokemonToParty(PlayerPokemon);
    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->CreateWildBattle(FPokemonDTO{.Species = TEXT("MACHOKE"), .Level = 50});

    auto Battlers = Battle->GetActiveBattlers();
    TScriptInterface<IBattler> Xerneas;
    TScriptInterface<IBattler> Opponent;
    for (auto Battler : Battlers) {
        if (Battler->GetNickname().ToString().Equals(TEXT("Xerneas"))) {
            Xerneas = Battler;
        } else if (Battler->GetNickname().ToString().Equals(TEXT("Machoke"))) {
            Opponent = Battler;
        }
    }
    UE_ASSERT_NOT_NULL(Xerneas.GetObject());
    UE_ASSERT_NOT_NULL(Opponent.GetObject());

    FDamageMultipliers Multipliers;
    auto AbilityModifiers = Xerneas->GetAbility()->GetDamageModifiers().Global;
    UE_ASSERT_FALSE(AbilityModifiers.IsEmpty());

    FMoveDamageInfo Context = {.User = Xerneas, .Type = TEXT("FAIRY")};
    AbilityModifiers[0]->Apply(Multipliers, Context);
    UE_CHECK_EQUAL(4.f / 3.f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto Zygarde = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("ZYGARDE"), .Level = 50, .Ability = FName(TEXT("AURABREAK"))});
    Opponent->Initialize(Opponent->GetOwningSide(), Zygarde);
    AbilityModifiers[0]->Apply(Multipliers, Context);
    UE_CHECK_EQUAL(0.75f, Multipliers.PowerMultiplier);

    return true;
}