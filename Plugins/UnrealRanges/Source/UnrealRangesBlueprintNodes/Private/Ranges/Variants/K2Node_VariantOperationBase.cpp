// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_VariantOperationBase.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Views/Filter.h"

FText UK2Node_VariantOperationBase::GetMenuCategory() const {
    return NSLOCTEXT("K2Node", "VariantOperationBase_GetMenuCategory", "Variants");
}

void UK2Node_VariantOperationBase::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    if (!ActionRegistrar.IsOpenForRegistration(GetClass())) {
        return;
    }

    auto &Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
    // clang-format off
    auto AllData = Registry.GetAllRegisteredStructs() |
                   UE::Ranges::Filter([](const UE::Ranges::IVariantRegistration &StructType) {
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