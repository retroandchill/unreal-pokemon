#include "PokemonDataEditor.h"
#include "Abilities/AbilityIndexCustomization.h"

#define LOCTEXT_NAMESPACE "FPokemonDataEditorModule"

void FPokemonDataEditorModule::StartupModule() {
    FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("AbilityIndex"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic( &FAbilityIndexCustomization::MakeInstance ));
}

void FPokemonDataEditorModule::ShutdownModule() {
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPokemonDataEditorModule, PokemonDataEditor)