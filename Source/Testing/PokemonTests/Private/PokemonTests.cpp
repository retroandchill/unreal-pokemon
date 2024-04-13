#include "PokemonTests.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "AutomationTestModule.h"
#include "MessageLogModule.h"
#include "TestShutdownSubsystem.h"

#define LOCTEXT_NAMESPACE "FPokemonTestsModule"

void FPokemonTestsModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FPokemonTestsModule::PostEngineInit);

    if (FModuleManager::Get().IsModuleLoaded("MessageLog")) {
        auto &MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
        MessageLogModule.RegisterLogListing("PokemonTests", LOCTEXT("PokemonTests", "Pokémon Tests"));
    }
}

void FPokemonTestsModule::ShutdownModule() {
    if (FModuleManager::Get().IsModuleLoaded("MessageLog")) {
        FMessageLogModule &MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
        MessageLogModule.UnregisterLogListing("PokemonTests");
    }
}

void FPokemonTestsModule::PostEngineInit() {
    if (GEngine != nullptr) {
        IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("ShutdownAfterTests"),
            TEXT("This calls for a full shutdown of the editor, so that LLVM can generate coverage profiling."),
            FConsoleCommandDelegate::CreateLambda(
                []() { UTestShutdownSubsystem::GetInstance().RequestExitAfterTestsComplete(); }));
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonTestsModule, PokemonTests)
