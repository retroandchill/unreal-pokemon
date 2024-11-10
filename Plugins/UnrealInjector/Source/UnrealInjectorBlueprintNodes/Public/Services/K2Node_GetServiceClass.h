// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "Services/Service.h"

#include "K2Node_GetServiceClass.generated.h"

/**
 * @class UK2Node_GetServiceClass
 * @brief This class is used to handle Blueprint nodes intended to get service class information.
 *
 * The UK2Node_GetServiceClass class is designed to interact with the Unreal Engine Blueprint system,
 * allowing users to dynamically obtain and manipulate service class data. This includes the provisioning
 * of services within the game's module system and is part of the engine's node-based programming framework.
 *
 * It primarily facilitates the retrieval of service class types and is often used in contexts where dynamic
 * class information is required at runtime or during the blueprint scripting phase.
 *
 * This class typically integrates with other Unreal Engine systems to provide a seamless user experience
 * in game development, especially within the context of Blueprint visual scripting.
 *
 * @brief Get the service class through Unreal Engine's Blueprint visual scripting system.
 */
UCLASS()
class UNREALINJECTORBLUEPRINTNODES_API UK2Node_GetServiceClass : public UK2Node {
    GENERATED_BODY()

  public:
    void Initialize(const TSubclassOf<UService> &Service);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    FText GetCompactNodeTitle() const override;
    bool ShouldDrawCompact() const override;
    bool IsNodePure() const override;
    FText GetMenuCategory() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  private:
    static void AddMenuActionForType(const TSubclassOf<UService> &Type,
                                     FBlueprintActionDatabaseRegistrar &ActionRegistrar, UClass *ActionKey);

    UPROPERTY()
    TSubclassOf<UService> ServiceClass;
};