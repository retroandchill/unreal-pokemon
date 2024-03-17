// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_ConstructObjectFromClass.h"
#include "K2Node_AddWidgetToStack.generated.h"

/**
 * Blueprint node for adding a menu widget to the stack
 */
UCLASS()
class RPGMENUSDEVELOPER_API UK2Node_AddWidgetToStack : public UK2Node_ConstructObjectFromClass {
	GENERATED_BODY()

public:
	explicit UK2Node_AddWidgetToStack(const FObjectInitializer& ObjectInitializer);

	void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	FText GetMenuCategory() const override;
	FName GetCornerIcon() const override;

protected:
	FText GetBaseNodeTitle() const override;
	FText GetNodeTitleFormat() const override;
	FText GetDefaultNodeTitle() const override;
	UClass* GetClassPinBaseClass() const override;
};
