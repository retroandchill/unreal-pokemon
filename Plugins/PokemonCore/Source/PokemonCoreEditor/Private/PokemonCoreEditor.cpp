#include "PokemonCoreEditor.h"
#include "Pokemon/PokemonDTODetails.h"

#define LOCTEXT_NAMESPACE "FPokemonCoreEditorModule"

void FPokemonCoreEditorModule::StartupModule() {
    
    FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("PokemonDTO"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic( &FPokemonDTODetails::MakeInstance ));
}

void FPokemonCoreEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPokemonCoreEditorModule, PokemonCoreEditor)