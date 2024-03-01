// "Unreal Pokémon" created by Retro & Chill.
#include "DataRetrieval/Nodes/K2Node_IsGameDataIDValid.h"

#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "DataRetrieval/DataUtilities.h"

void UK2Node_IsGameDataIDValid::Initialize(const UScriptStruct* NodeStruct) {
	StructType = NodeStruct;
}

void UK2Node_IsGameDataIDValid::AllocateDefaultPins() {
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, TEXT("RowName"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, UEdGraphSchema_K2::PN_ReturnValue);

	Super::AllocateDefaultPins();
}

FText UK2Node_IsGameDataIDValid::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	if (StructType) {
		if (TitleType == ENodeTitleType::FullTitle) {
			return StructType->GetDisplayNameText();
		}

		const FString& StructName = StructType->GetName();
		return FText::FormatNamed(
			NSLOCTEXT("K2Node", "IsGameDataIDValid_NodeTitleFormat", "Is {ClassName} ID Valid?"), TEXT("ClassName"),
			FText::FromString(StructName));
	}

	return GetTooltipText();
}

FText UK2Node_IsGameDataIDValid::GetTooltipText() const {
	if (StructType) {
		return FText::FormatNamed(NSLOCTEXT("K2Node", "IsGameDataIDValid_TooltipFormat",
		                                    "Is {StructName} ID Valid? \n\n{StructTooltip}"),
		                          TEXT("StructName"), StructType->GetDisplayNameText(),
		                          TEXT("StructTooltip"), StructType->GetToolTipText(/*bShortTooltip=*/ true));
	}

	return NSLOCTEXT("K2Node", "IsGameDataIDValid_InvalidStructTypeTooltip", "Invalid Struct Type");
}

bool UK2Node_IsGameDataIDValid::IsNodePure() const {
	return true;
}

FText UK2Node_IsGameDataIDValid::GetMenuCategory() const {
	if (StructType == nullptr)
		return Super::GetMenuCategory();

	FString FullCategory = "Data";
	if (auto MetaTag = TEXT("DatabaseType"); StructType->HasMetaData(MetaTag))
		FullCategory += "|" + StructType->GetMetaData(MetaTag);

	FullCategory += "|" + StructType->GetDisplayNameText().ToString();
	return FText::FromString(FullCategory);
}

FSlateIcon UK2Node_IsGameDataIDValid::GetIconAndTint(FLinearColor& OutColor) const {
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_IsGameDataIDValid::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	Super::GetMenuActions(ActionRegistrar);

	UDataUtilities::AddAllDataTableTypesToMenu<UK2Node_IsGameDataIDValid>(GetClass(), ActionRegistrar);
}

void UK2Node_IsGameDataIDValid::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UDataUtilities, IsDataRowNameValid);
	auto CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallGetNode->FunctionReference.SetExternalMember(FunctionName, UDataUtilities::StaticClass());
	CallGetNode->AllocateDefaultPins();
	
	static const FName StructType_ParamName(TEXT("StructType"));
	static const FName RowName_ParamName(TEXT("RowName"));
	
	auto RowNamePin = FindPinChecked(TEXT("RowName"));
	auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	
	auto CallCreateStructTypePin = CallGetNode->FindPinChecked(StructType_ParamName);
	auto CallCreateRowNamePin = CallGetNode->FindPinChecked(RowName_ParamName);
	auto CallCreateOutRowPin = CallGetNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	CallCreateStructTypePin->DefaultObject = const_cast<UScriptStruct*>(StructType.Get());
	CompilerContext.MovePinLinksToIntermediate(*RowNamePin, *CallCreateRowNamePin);

	CallCreateOutRowPin->PinType = ReturnValuePin->PinType;
	CallCreateOutRowPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateOutRowPin);

	BreakAllNodeLinks();
}
