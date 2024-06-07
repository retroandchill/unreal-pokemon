// "Unreal Pokémon" created by Retro & Chill.

#include "UI/K2Node_SelectPokemonFromParty.h"
#include "Nodes/SelectPokemonFromParty.h"
#include "Screens/PokemonSelectScreen.h"

void UK2Node_SelectPokemonFromParty::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto FactoryFunc = USelectPokemonFromParty::StaticClass()->FindFunctionByName("SelectPokemonFromParty");
    check(FactoryFunc != nullptr)
    SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_SelectPokemonFromParty::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_SelectPokemonFromParty::ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const {
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf(UPokemonSelectScreen::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract)) {
            continue;
        }

        Action(*It);
    }
}