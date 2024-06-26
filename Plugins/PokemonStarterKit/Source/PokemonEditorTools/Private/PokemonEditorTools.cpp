#include "PokemonEditorTools.h"
#include "Details/PocketKeyCustomization.h"

void FPokemonEditorToolsModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("PocketKey"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FPocketKeyCustomization::MakeInstance));
}

void FPokemonEditorToolsModule::ShutdownModule() {
    // No shutdown required
}

IMPLEMENT_MODULE(FPokemonEditorToolsModule, PokemonEditorTools)