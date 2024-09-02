#include "PokemonEditorTools.h"
#include "Bag/Item.h"
#include "Battle/Status.h"
#include "Details/DataHandleCustomization.h"
#include "Details/WrappedKeyCustomization.h"
#include "Moves/MoveData.h"
#include "Pins/DataHandlePinFactory.h"
#include "Species/Ability.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include <functional>

#define REGISTER_WRAPPED_PROPERTY(PropertyModule, StructName, PropertyName) \
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT(#StructName), \
        FOnGetPropertyTypeCustomizationInstance::CreateLambda( \
            std::bind_front(&FWrappedKeyCustomization::MakeInstance, \
                GET_MEMBER_NAME_CHECKED(F##StructName, PropertyName))))

void FPokemonEditorToolsModule::StartupModule() {
    FEdGraphUtilities::RegisterVisualPinFactory(MakeShared<FDataHandlePinFactory>());
    
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("DataStructHandle"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataHandleCustomization::MakeInstance));
    
    REGISTER_WRAPPED_PROPERTY(PropertyModule, PocketKey, PocketName);
    
    REGISTER_WRAPPED_PROPERTY(PropertyModule, ItemHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, StatusHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, StatHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, MainStatHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, MainBattleStatHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, BattleStatHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, SpeciesHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, MoveHandle, RowID);
    REGISTER_WRAPPED_PROPERTY(PropertyModule, AbilityHandle, RowID);
}

void FPokemonEditorToolsModule::ShutdownModule() {
    // No shutdown required
}

IMPLEMENT_MODULE(FPokemonEditorToolsModule, PokemonEditorTools)