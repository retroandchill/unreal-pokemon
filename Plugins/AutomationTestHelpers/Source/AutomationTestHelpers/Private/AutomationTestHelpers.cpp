// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTestHelpers.h"
#include "TestShutdownSubsystem.h"
#include "Developer/MessageLog/Public/MessageLogModule.h"

#define LOCTEXT_NAMESPACE "FAutomationTestHelpersModule"

class FMessageLogModule;

void FAutomationTestHelpersModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FAutomationTestHelpersModule::PostEngineInit);

    if (FModuleManager::Get().IsModuleLoaded("MessageLog")) {
        auto &MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
        MessageLogModule.RegisterLogListing("PokemonTests", LOCTEXT("PokemonTests", "Pokémon Tests"));
    }
}

void FAutomationTestHelpersModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("MessageLog")) {
        FMessageLogModule &MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
        MessageLogModule.UnregisterLogListing("PokemonTests");
    }
}

void FAutomationTestHelpersModule::PostEngineInit() {
    if (GEngine != nullptr) {
        IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("ShutdownAfterTests"),
            TEXT("This calls for a full shutdown of the editor, so that LLVM can generate coverage profiling."),
            FConsoleCommandDelegate::CreateLambda(
                []() { UTestShutdownSubsystem::GetInstance().RequestExitAfterTestsComplete(); }));
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutomationTestHelpersModule, AutomationTestHelpers)