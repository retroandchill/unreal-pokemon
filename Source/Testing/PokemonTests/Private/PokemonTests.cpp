#include "PokemonTests.h"

#include "AutomationBlueprintFunctionLibrary.h"
#include "AutomationTestModule.h"
#include "TestShutdownSubsystem.h"

#define LOCTEXT_NAMESPACE "FPokemonTestsModule"

void FPokemonTestsModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FPokemonTestsModule::PostEngineInit);
}

void FPokemonTestsModule::ShutdownModule() {
    // No special teardown needed
}

void FPokemonTestsModule::PostEngineInit() {
    if (GEngine != nullptr) {
        IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("FullShutdown"),
            TEXT("This calls for a full shutdown of the editor, so that LLVM can generate coverage profiling."),
            FConsoleCommandDelegate::CreateLambda([]() {
                UTestShutdownSubsystem::GetInstance().RequestExitAfterTestsComplete();
            }));
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonTestsModule, PokemonTests)
