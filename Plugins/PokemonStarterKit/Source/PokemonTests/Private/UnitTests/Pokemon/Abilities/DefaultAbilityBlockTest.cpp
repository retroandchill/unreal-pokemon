
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultAbilityBlockTest,
                                 "Private.UnitTests.Core.Pokemon.Abilities.DefaultAbilityBlockTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultAbilityBlockTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    FPokemonDTO Pokemon1 = {.Species = FName("LUCARIO"), .Level = 78, .PersonalityValue = 0x39593A01};
    auto NewPokemon1 = UGamePokemon::Create(World.Get(), Pokemon1);
    auto Block1 = NewObject<UDefaultAbilityBlock>();
    Block1->Initialize(NewPokemon1, Pokemon1);
    CHECK_EQUAL(FName("INNERFOCUS"), Block1->GetAbilityID());
    CHECK_EQUAL(TEXT("Inner Focus"), Block1->GetDisplayName().ToString());
    CHECK_EQUAL(TEXT("The Pokémon is protected from flinching."), Block1->GetAbilityDescription().ToString());

    FPokemonDTO Pokemon2 = {.Species = FName("LUCARIO"), .Level = 78, .PersonalityValue = 0x39593A02};
    auto NewPokemon2 = UGamePokemon::Create(World.Get(), Pokemon2);
    auto Block2 = NewObject<UDefaultAbilityBlock>();
    Block2->Initialize(NewPokemon2, Pokemon2);
    CHECK_EQUAL(FName("STEADFAST"), Block2->GetAbilityID());
    CHECK_EQUAL(TEXT("Steadfast"), Block2->GetDisplayName().ToString());
    CHECK_EQUAL(TEXT("Raises Speed each time the Pokémon flinches."), Block2->GetAbilityDescription().ToString());

    FPokemonDTO Pokemon3 = {.Species = FName("LUCARIO"), .Level = 78, .Ability = FName("JUSTIFIED")};
    auto NewPokemon3 = UGamePokemon::Create(World.Get(), Pokemon3);
    auto Block3 = NewObject<UDefaultAbilityBlock>();
    Block3->Initialize(NewPokemon3, Pokemon3);
    CHECK_EQUAL(FName("JUSTIFIED"), Block3->GetAbilityID());
    CHECK_EQUAL(TEXT("Justified"), Block3->GetDisplayName().ToString());
    CHECK_EQUAL(TEXT("Raises Attack when hit by a Dark-type move."), Block3->GetAbilityDescription().ToString());

    FPokemonDTO Pokemon4 = {.Species = FName("LUCARIO"), .Level = 78, .Ability = FName()};
    auto NewPokemon4 = UGamePokemon::Create(World.Get(), Pokemon4);
    auto Block4 = NewObject<UDefaultAbilityBlock>();
    Block4->Initialize(NewPokemon4, Pokemon4);
    CHECK_EQUAL(FName(), Block4->GetAbilityID());
    CHECK_EQUAL(TEXT("No Ability"), Block4->GetDisplayName().ToString());
    CHECK_EQUAL(TEXT("This Pokémon has no ability."), Block4->GetAbilityDescription().ToString());

    return true;
}