// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_VariantOperationBase.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Ranges/Views/Filter.h"

FText UK2Node_VariantOperationBase::GetMenuCategory() const {
    return NSLOCTEXT("K2Node", "VariantOperationBase_GetMenuCategory", "Variants");
}

bool UK2Node_VariantOperationBase::IsNodePure() const {
    return true;
}

FSlateIcon UK2Node_VariantOperationBase::GetIconAndTint(FLinearColor &OutColor) const {
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_VariantOperationBase::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    if (!ActionRegistrar.IsOpenForRegistration(GetClass())) {
        return;
    }

    auto &Registry = Retro::FVariantObjectStructRegistry::Get();
    // clang-format off
    auto AllData = Registry.GetAllRegisteredStructs() |
                   Retro::Ranges::Views::Filter([](const Retro::IVariantRegistration &StructType) {
                       return UEdGraphSchema_K2::IsAllowableBlueprintVariableType(
                           StructType.GetStructType(), true);
                   });
    // clang-format on
    for (TSet<UScriptStruct *> Seen; auto &Registration : AllData) {
        auto Struct = Registration.GetStructType();
        if (Seen.Contains(Struct)) {
            continue;
        }
        Seen.Add(Struct);
        AddMenuOptionsForStruct(ActionRegistrar, Registration);
    }
}