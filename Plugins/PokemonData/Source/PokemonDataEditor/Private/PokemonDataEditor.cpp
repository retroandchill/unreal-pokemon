#include "PokemonDataEditor.h"
#include "Details/PocketKeyCustomization.h"

void FPokemonDataEditorModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("PocketKey"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FPocketKeyCustomization::MakeInstance));
}

void FPokemonDataEditorModule::ShutdownModule() {
    // No shutdown needed
}
    
IMPLEMENT_MODULE(FPokemonDataEditorModule, PokemonDataEditor)