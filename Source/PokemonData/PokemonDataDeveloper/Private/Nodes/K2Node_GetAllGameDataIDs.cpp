// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/K2Node_GetAllGameDataIDs.h"

#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "DataRetrieval/DataUtilities.h"

void UK2Node_GetAllGameDataIDs::Initialize(const UScriptStruct* NodeStruct) {
	StructType = NodeStruct;
}

void UK2Node_GetAllGameDataIDs::AllocateDefaultPins() {
	FCreatePinParams ReturnTypePinParams;
	ReturnTypePinParams.ContainerType = EPinContainerType::Array;
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Name, UEdGraphSchema_K2::PN_ReturnValue, ReturnTypePinParams);

	Super::AllocateDefaultPins();
}

FText UK2Node_GetAllGameDataIDs::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	if (StructType) {
		if (TitleType == ENodeTitleType::FullTitle) {
			return StructType->GetDisplayNameText();
		}

		const FString& StructName = StructType->GetName();
		return FText::FormatNamed(
			NSLOCTEXT("K2Node", "GetAllGameDataIDs_NodeTitleFormat", "Get All {ClassName} IDs"), TEXT("ClassName"),
			FText::FromString(StructName));
	}

	return Super::GetNodeTitle(TitleType);
}

FText UK2Node_GetAllGameDataIDs::GetTooltipText() const {
	if (StructType) {
		return FText::FormatNamed(NSLOCTEXT("K2Node", "GetAllGameDataIDs_TooltipFormat",
		                                    "Get All {StructName} IDs \n\n{StructTooltip}"),
		                          TEXT("StructName"), StructType->GetDisplayNameText(),
		                          TEXT("StructTooltip"), StructType->GetToolTipText(/*bShortTooltip=*/ true));
	}

	return NSLOCTEXT("K2Node", "GetAllGameDataIDs_InvalidStructTypeTooltip", "Invalid Struct Type");
}

bool UK2Node_GetAllGameDataIDs::IsNodePure() const {
	return true;
}

FText UK2Node_GetAllGameDataIDs::GetMenuCategory() const {
	if (StructType == nullptr)
		return Super::GetMenuCategory();

	FString FullCategory = "Data";
	if (auto MetaTag = TEXT("DatabaseType"); StructType->HasMetaData(MetaTag))
		FullCategory += "|" + StructType->GetMetaData(MetaTag);

	FullCategory += "|" + StructType->GetDisplayNameText().ToString();
	return FText::FromString(FullCategory);
}

FSlateIcon UK2Node_GetAllGameDataIDs::GetIconAndTint(FLinearColor& OutColor) const {
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetAllGameDataIDs::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	Super::GetMenuActions(ActionRegistrar);

	UDataUtilities::AddAllDataTableTypesToMenu<UK2Node_GetAllGameDataIDs>(GetClass(), ActionRegistrar);
}

void UK2Node_GetAllGameDataIDs::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UDataUtilities, GetAllDataIDs);
	auto CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallGetNode->FunctionReference.SetExternalMember(FunctionName, UDataUtilities::StaticClass());
	CallGetNode->AllocateDefaultPins();

	static const FName StructType_ParamName(TEXT("StructType"));

	auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	auto CallCreateStructTypePin = CallGetNode->FindPinChecked(StructType_ParamName);
	auto CallCreateReturnValuePin = CallGetNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	CallCreateStructTypePin->DefaultObject = const_cast<UScriptStruct*>(StructType.Get());
	CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateReturnValuePin);

	BreakAllNodeLinks();
}
