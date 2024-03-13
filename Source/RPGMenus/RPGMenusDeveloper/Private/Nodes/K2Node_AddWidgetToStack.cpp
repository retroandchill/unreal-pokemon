// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/K2Node_AddWidgetToStack.h"

#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "KismetCompilerMisc.h"
#include "RPGPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Screens/Screen.h"

UK2Node_AddWidgetToStack::UK2Node_AddWidgetToStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	NodeTooltip = NSLOCTEXT("UK2Node_AddWidgetToStack", "NodeTooltip", "Creates a new widget");
}

void UK2Node_AddWidgetToStack::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);

	static const FName Create_FunctionName = GET_FUNCTION_NAME_CHECKED(ARPGPlayerController, AddScreenToStackHelper);
	static const FName WorldContextObject_ParamName(TEXT("WorldContextObject"));
	static const FName WidgetType_ParamName(TEXT("ScreenType"));

	UK2Node_AddWidgetToStack* CreateWidgetNode = this;
	UEdGraphPin* SpawnNodeExec = CreateWidgetNode->GetExecPin();
	UEdGraphPin* SpawnWorldContextPin = CreateWidgetNode->GetWorldContextPin();
	UEdGraphPin* SpawnClassPin = CreateWidgetNode->GetClassPin();
	UEdGraphPin* SpawnNodeThen = CreateWidgetNode->GetThenPin();
	UEdGraphPin* SpawnNodeResult = CreateWidgetNode->GetResultPin();

	UClass* SpawnClass = (SpawnClassPin != nullptr) ? Cast<UClass>(SpawnClassPin->DefaultObject) : nullptr;
	if (!SpawnClassPin || ((0 == SpawnClassPin->LinkedTo.Num()) && (nullptr == SpawnClass))) {
		CompilerContext.MessageLog.Error(*NSLOCTEXT("UK2Node_AddWidgetToStack", "CreateWidgetNodeMissingClass_Error",
		                                            "Spawn node @@ must have a class specified.").ToString(),
		                                 CreateWidgetNode);
		// we break exec links so this is the only error we get, don't want the CreateWidget node being considered and giving 'unexpected node' type warnings
		CreateWidgetNode->BreakAllNodeLinks();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// create 'UWidgetBlueprintLibrary::Create' call node
	UK2Node_CallFunction* CallCreateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(
		CreateWidgetNode, SourceGraph);
	CallCreateNode->FunctionReference.SetExternalMember(Create_FunctionName, ARPGPlayerController::StaticClass());
	CallCreateNode->AllocateDefaultPins();

	// store off the class to spawn before we mutate pin connections:
	UClass* ClassToSpawn = GetClassToSpawn();

	UEdGraphPin* CallCreateExec = CallCreateNode->GetExecPin();
	UEdGraphPin* CallCreateWorldContextPin = CallCreateNode->FindPinChecked(WorldContextObject_ParamName);
	UEdGraphPin* CallCreateWidgetTypePin = CallCreateNode->FindPinChecked(WidgetType_ParamName);
	UEdGraphPin* CallCreateResult = CallCreateNode->GetReturnValuePin();

	// Move 'exec' connection from create widget node to 'UWidgetBlueprintLibrary::Create'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeExec, *CallCreateExec);

	if (SpawnClassPin->LinkedTo.Num() > 0) {
		// Copy the 'blueprint' connection from the spawn node to 'UWidgetBlueprintLibrary::Create'
		CompilerContext.MovePinLinksToIntermediate(*SpawnClassPin, *CallCreateWidgetTypePin);
	} else {
		// Copy blueprint literal onto 'UWidgetBlueprintLibrary::Create' call
		CallCreateWidgetTypePin->DefaultObject = SpawnClass;
	}

	// Copy the world context connection from the spawn node to 'UWidgetBlueprintLibrary::Create' if necessary
	if (SpawnWorldContextPin) {
		CompilerContext.MovePinLinksToIntermediate(*SpawnWorldContextPin, *CallCreateWorldContextPin);
	}

	// Move result connection from spawn node to 'UWidgetBlueprintLibrary::Create'
	CallCreateResult->PinType = SpawnNodeResult->PinType; // Copy type so it uses the right actor subclass
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeResult, *CallCreateResult);

	//////////////////////////////////////////////////////////////////////////
	// create 'set var' nodes

	// Get 'result' pin from 'begin spawn', this is the actual actor we want to set properties on
	UEdGraphPin* LastThen = FKismetCompilerUtilities::GenerateAssignmentNodes(
		CompilerContext, SourceGraph, CallCreateNode, CreateWidgetNode, CallCreateResult, ClassToSpawn);

	// Move 'then' connection from create widget node to the last 'then'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeThen, *LastThen);

	// Break any links to the expanded node
	CreateWidgetNode->BreakAllNodeLinks();
}

FText UK2Node_AddWidgetToStack::GetMenuCategory() const {
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::UserInterface);
}

FName UK2Node_AddWidgetToStack::GetCornerIcon() const {
	return TEXT("Graph.Replication.ClientEvent");
}

FText UK2Node_AddWidgetToStack::GetBaseNodeTitle() const {
	return NSLOCTEXT("UK2Node_AddWidgetToStack", "AddWidgetToStack_BaseTitle", "Add Screen to Stack");
}

FText UK2Node_AddWidgetToStack::GetNodeTitleFormat() const {
	return NSLOCTEXT("UK2Node_AddWidgetToStack", "CreateWidget", "Add {ClassName} to Stack");
}

UClass* UK2Node_AddWidgetToStack::GetClassPinBaseClass() const {
	return UScreen::StaticClass();
}
