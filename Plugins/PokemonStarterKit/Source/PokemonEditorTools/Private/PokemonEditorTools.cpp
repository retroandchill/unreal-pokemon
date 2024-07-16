#include "PokemonEditorTools.h"
#include "Details/WrappedKeyCustomization.h"
#include <functional>

void FPokemonEditorToolsModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("PocketKey"), FOnGetPropertyTypeCustomizationInstance::CreateLambda(
                               std::bind_front(&FWrappedKeyCustomization::MakeInstance, "PocketName")));
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("BattleStat"), FOnGetPropertyTypeCustomizationInstance::CreateLambda(
                                std::bind_front(&FWrappedKeyCustomization::MakeInstance, "Stat")));
}

void FPokemonEditorToolsModule::ShutdownModule() {
    // No shutdown required
}

IMPLEMENT_MODULE(FPokemonEditorToolsModule, PokemonEditorTools)