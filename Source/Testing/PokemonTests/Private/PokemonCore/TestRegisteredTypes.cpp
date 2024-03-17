﻿// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonCoreSettings.h"
#include "Misc/AutomationTest.h"

#include "Utilities/accessor.hpp"

using namespace accessor;

struct FOptionsParams {
	TArray<FName> RetVal;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestRegisteredTypes, "Project.PokemonCore.TestRegisteredTypes",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestRegisteredTypes::RunTest(const FString& Parameters) {
	auto Settings = NewObject<UPokemonCoreSettings>();
	auto PokemonClassOptions = UPokemonCoreSettings::StaticClass()->FindFunctionByName(TEXT("GetPokemonClassOptions"));
	FOptionsParams PokemonParams;
	Settings->ProcessEvent(PokemonClassOptions, &PokemonParams);
	bool bPassed = TestTrue(TEXT("Pokémon Class should be registered"),
		PokemonParams.RetVal.Contains(Settings->GetPokemonClass()));
	
	auto StatBlockClassOptions = UPokemonCoreSettings::StaticClass()->FindFunctionByName(TEXT("GetStatBlockClassOptions"));
	FOptionsParams StatBlockParams;
	Settings->ProcessEvent(StatBlockClassOptions, &StatBlockParams);
	bPassed &= TestTrue(TEXT("Pokémon Class should be registered"),
		StatBlockParams.RetVal.Contains(Settings->GetStatBlockClass()));
	
	return bPassed;
}
