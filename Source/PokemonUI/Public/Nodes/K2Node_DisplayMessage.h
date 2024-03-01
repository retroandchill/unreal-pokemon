// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_DisplayMessageBase.h"
#include "K2Node_DisplayMessage.generated.h"

class UTextDisplayScreen;
/**
 * Blueprint node for handling the display of a message to the player
 */
UCLASS()
class POKEMONUI_API UK2Node_DisplayMessage : public UK2Node_DisplayMessageBase {
	GENERATED_BODY()

public:
	/**
	 * Default construct the class using the given initializer
	 * @param ObjectInitializer The Unreal provided initializer
	 */
	explicit UK2Node_DisplayMessage(const FObjectInitializer& ObjectInitializer);

	void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	
};
