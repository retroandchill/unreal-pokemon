// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/K2Node_DisplayMessageBase.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RPGPlayerController.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/TextDisplayScreen.h"

UK2Node_DisplayMessageBase::UK2Node_DisplayMessageBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintAsyncActionBase, Activate);
}

void UK2Node_DisplayMessageBase::Initialize(TSubclassOf<UTextDisplayScreen> NodeClass, TSharedRef<uint32> NodeCounter) {
	ScreenType = MoveTemp(NodeClass);
	TotalScreens = MoveTemp(NodeCounter);
}

void UK2Node_DisplayMessageBase::AllocateDefaultPins() {
	Super::AllocateDefaultPins();
	static const FName ScreenType_ParamName(TEXT("ScreenClass"));
	auto ScreenClassPin = FindPinChecked(ScreenType_ParamName);
	ScreenClassPin->DefaultObject = ScreenType;
	ScreenClassPin->bHidden = true;
}

FText UK2Node_DisplayMessageBase::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	if (ScreenType != nullptr && *TotalScreens > 1) {
		const auto& ScreenName = ScreenType->GetName();
		return FText::FormatNamed(
			NSLOCTEXT("K2Node", "DisplayMessage_NodeTitleFormat", "{OriginalName} ({ClassName})"), TEXT("OriginalName"),
				Super::GetNodeTitle(TitleType), TEXT("ClassName"), FText::FromString(ScreenName));
	}

	return Super::GetNodeTitle(TitleType);
}

void UK2Node_DisplayMessageBase::SupplyMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar, UFunction* FactoryFunc) const {
	auto CustomizeCallback = [](UEdGraphNode* Node, [[maybe_unused]] bool bIsTemplateNode,
								TSubclassOf<UTextDisplayScreen> Subclass, TSharedRef<uint32> NodeCounter, UFunction *FactoryFunc) {
		
		auto TypedNode = CastChecked<UK2Node_DisplayMessageBase>(Node);
		auto ReturnProp = CastFieldChecked<FObjectProperty>(FactoryFunc->GetReturnProperty());
						
		TypedNode->ProxyFactoryFunctionName = FactoryFunc->GetFName();
		TypedNode->ProxyFactoryClass        = FactoryFunc->GetOuterUClass();
		TypedNode->ProxyClass               = ReturnProp->PropertyClass;
		TypedNode->Initialize(Subclass, MoveTemp(NodeCounter));
	};

	auto ScreenCounter = MakeShared<uint32>(0);
	for (TObjectIterator<UClass> It; It; ++It) {
		if (!It->IsChildOf(UTextDisplayScreen::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract))
			continue;
		
		if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(*It, true))
			continue;

		auto Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);

		(*ScreenCounter)++;
		Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, TSubclassOf<UTextDisplayScreen>(*It), ScreenCounter, FactoryFunc);
		ActionRegistrar.AddBlueprintAction(GetClass(), Spawner);
	}
}

void UK2Node_DisplayMessageBase::ReconnectOutputPin(FKismetCompilerContext& CompilerContext,
                                                    UEdGraphPin* OutputPin) {
	if (OutputPin->LinkedTo.ContainsByPredicate([](UEdGraphPin* Link) { return Link->GetOwningNode()->GetClass()->ImplementsInterface(UMessageNode::StaticClass()); }))
		return;
	
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED_OneParam(ARPGPlayerController, RemoveScreenFromStack, UObject*);
	auto IntermediateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, GetGraph());
	IntermediateNode->FunctionReference.SetExternalMember(FunctionName, ARPGPlayerController::StaticClass());
	IntermediateNode->AllocateDefaultPins();

	if (auto This_WorldContextPin = FindPin(TEXT("WorldContextObject")); This_WorldContextPin != nullptr) {
		auto Intermediate_WorldContextPin = IntermediateNode->FindPinChecked(TEXT("WorldContextObject"));
		CompilerContext.CopyPinLinksToIntermediate(*This_WorldContextPin, *Intermediate_WorldContextPin);
	}

	auto InputPin = IntermediateNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
	auto ThenPin = IntermediateNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	
	CompilerContext.MovePinLinksToIntermediate(*OutputPin, *ThenPin);
	OutputPin->MakeLinkTo(InputPin);
}
