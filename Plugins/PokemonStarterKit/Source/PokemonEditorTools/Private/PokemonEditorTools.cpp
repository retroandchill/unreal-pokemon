#include "PokemonEditorTools.h"
#include "Battle/GameplayAbilities/Events/BattleEvent.h"
#include "Details/EventValueCustomization.h"
#include "Details/PocketKeyCustomization.h"

void FPokemonEditorToolsModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("PocketKey"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FPocketKeyCustomization::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("EventValue"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FEventValueCustomization::MakeInstance, FBattleEventValues::StaticStruct()));
}

void FPokemonEditorToolsModule::ShutdownModule() {
    // No shutdown required
}

IMPLEMENT_MODULE(FPokemonEditorToolsModule, PokemonEditorTools)