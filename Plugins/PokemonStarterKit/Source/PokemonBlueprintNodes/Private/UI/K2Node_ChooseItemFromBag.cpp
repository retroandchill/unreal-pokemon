// "Unreal Pokémon" created by Retro & Chill.

#include "UI/K2Node_ChooseItemFromBag.h"
#include "Nodes/ChooseItemFromBag.h"
#include "Screens/BagScreen.h"

void UK2Node_ChooseItemFromBag::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto FactoryFunc = UChooseItemFromBag::StaticClass()->FindFunctionByName("ChooseItemFromBag");
    check(FactoryFunc != nullptr)
    SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_ChooseItemFromBag::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_ChooseItemFromBag::ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const {
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf(UBagScreen::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract)) {
            continue;
        }

        Action(*It);
    }
}