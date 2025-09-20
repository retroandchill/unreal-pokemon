#include "PokemonEditorTools.h"
#include "Details/EvolutionConditionDetails.h"

void FPokemonEditorToolsModule::StartupModule()
{
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    EvolutionConditionPropertyIdentifier = MakeShared<FEvolutionConditionPropertyIdentifier>();
    PropertyModule.RegisterCustomPropertyTypeLayout(
        "StructProperty",
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEvolutionConditionDetails::MakeInstance),
        EvolutionConditionPropertyIdentifier);
}

void FPokemonEditorToolsModule::ShutdownModule()
{
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.UnregisterCustomPropertyTypeLayout("StructProperty", EvolutionConditionPropertyIdentifier);
}

IMPLEMENT_MODULE(FPokemonEditorToolsModule, PokemonEditorTools)