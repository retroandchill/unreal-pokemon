//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "DataRetrieval/Nodes/K2Node_GetAllGameDataIDs.h"

#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "DataRetrieval/DataUtilities.h"

void UK2Node_GetAllGameDataIDs::Initialize(const UScriptStruct* NodeStruct) {
	StructType = NodeStruct;
}

void UK2Node_GetAllGameDataIDs::AllocateDefaultPins() {
	if (GetBlueprint()->ParentClass->HasMetaData(FBlueprintMetadata::MD_ShowWorldContextPin)) {
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), TEXT("WorldContext"));
	}

	FCreatePinParams ReturnTypePinParams;
	ReturnTypePinParams.ContainerType = EPinContainerType::Set;
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

	static const FName WorldContextObject_ParamName(TEXT("ContextObject"));
	static const FName StructType_ParamName(TEXT("StructType"));

	auto SpawnWorldContextPin = FindPin(TEXT("WorldContext"));
	auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	auto CallCreateWorldContextPin = CallGetNode->FindPinChecked(WorldContextObject_ParamName);
	auto CallCreateStructTypePin = CallGetNode->FindPinChecked(StructType_ParamName);
	auto CallCreateReturnValuePin = CallGetNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	// Copy the world context connection from the spawn node to 'USubsystemBlueprintLibrary::Get[something]Subsystem' if necessary
	if (SpawnWorldContextPin != nullptr) {
		CompilerContext.MovePinLinksToIntermediate(*SpawnWorldContextPin, *CallCreateWorldContextPin);
	}

	CallCreateStructTypePin->DefaultObject = const_cast<UScriptStruct*>(StructType.Get());
	CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateReturnValuePin);

	BreakAllNodeLinks();
}
