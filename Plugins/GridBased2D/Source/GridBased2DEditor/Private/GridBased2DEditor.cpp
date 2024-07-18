// "Unreal Pokémon" created by Retro & Chill.
#include "GridBased2DEditor.h"
#include "Details/GridBasedMapDetails.h"

constexpr auto GLoctextNamespace = "FGridBased2DEditorModule";

void FGridBased2DEditorModule::StartupModule() {
    FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(
        "GridBasedMap", FOnGetDetailCustomizationInstance::CreateStatic(&FGridBasedMapDetails::MakeInstance));
}

void FGridBased2DEditorModule::ShutdownModule() {
    // No special shutdown required
}

IMPLEMENT_MODULE(FGridBased2DEditorModule, GridBased2DEditor)
