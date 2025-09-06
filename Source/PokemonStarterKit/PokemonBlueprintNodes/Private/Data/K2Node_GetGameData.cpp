// "Unreal Pokémon" created by Retro & Chill.
#include "Data/K2Node_GetGameData.h"
#include "DataRetrieval/DataUtilities.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"

void UK2Node_GetGameData::Initialize(UScriptStruct *NodeStruct)
{
    StructType = NodeStruct;
}

void UK2Node_GetGameData::AllocateDefaultPins()
{
    auto RowNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, TEXT("RowName"));
    RowNamePin->DefaultValue = GetRowNames()[0].ToString();
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              (StructType != nullptr ? static_cast<UStruct *>(StructType) : FTableRowBase::StaticStruct()),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_GetGameData::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (StructType)
    {
        if (TitleType == ENodeTitleType::FullTitle)
        {
            return StructType->GetDisplayNameText();
        }

        return FText::FormatNamed(NSLOCTEXT("K2Node", "GetGameData_NodeTitleFormat", "Get {ClassName}"),
                                  TEXT("ClassName"), StructType->GetDisplayNameText());
    }

    return GetTooltipText();
}

FText UK2Node_GetGameData::GetTooltipText() const
{
    if (StructType)
    {
        return FText::FormatNamed(
            NSLOCTEXT("K2Node", "GetGameData_TooltipFormat", "Get {StructName} \n\n{StructTooltip}"),
            TEXT("StructName"), StructType->GetDisplayNameText(), TEXT("StructTooltip"),
            StructType->GetToolTipText(/*bShortTooltip=*/true));
    }

    return NSLOCTEXT("K2Node", "GetGameData_InvalidStructTypeTooltip", "Invalid Struct Type");
}

bool UK2Node_GetGameData::IsNodePure() const
{
    return true;
}

FText UK2Node_GetGameData::GetMenuCategory() const
{
    if (StructType == nullptr)
        return Super::GetMenuCategory();

    FString FullCategory = "Data";
    if (auto MetaTag = TEXT("DatabaseType"); StructType->HasMetaData(MetaTag))
        FullCategory += "|" + StructType->GetMetaData(MetaTag);

    FullCategory += "|" + StructType->GetDisplayNameText().ToString();
    return FText::FromString(FullCategory);
}

FSlateIcon UK2Node_GetGameData::GetIconAndTint(FLinearColor &OutColor) const
{
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_GetGameData::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const
{
    Super::GetMenuActions(ActionRegistrar);

    UDataUtilities::AddAllDataTableTypesToMenu<UK2Node_GetGameData>(GetClass(), ActionRegistrar);
}

void UK2Node_GetGameData::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UDataUtilities, GetData);
    auto CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallGetNode->FunctionReference.SetExternalMember(FunctionName, UDataUtilities::StaticClass());
    CallGetNode->AllocateDefaultPins();

    static const FName StructType_ParamName(TEXT("StructType"));
    static const FName RowName_ParamName(TEXT("RowName"));
    static const FName OutRow_ParamName(TEXT("OutRow"));

    auto RowNamePin = FindPinChecked(TEXT("RowName"));
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateStructTypePin = CallGetNode->FindPinChecked(StructType_ParamName);
    auto CallCreateRowNamePin = CallGetNode->FindPinChecked(RowName_ParamName);
    auto CallCreateOutRowPin = CallGetNode->FindPinChecked(OutRow_ParamName);

    CallCreateStructTypePin->DefaultObject = StructType.Get();
    CompilerContext.MovePinLinksToIntermediate(*RowNamePin, *CallCreateRowNamePin);

    CallCreateOutRowPin->PinType = ReturnValuePin->PinType;
    CallCreateOutRowPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateOutRowPin);

    BreakAllNodeLinks();
}

TArray<FName> UK2Node_GetGameData::GetRowNames() const
{
    if (StructType == nullptr)
        return {};

    auto &DataTable = FDataManager::GetInstance().GetDataTable(StructType);
    return DataTable.GetTableRowNames();
}

UEdGraphPin *UK2Node_GetGameData::GetRowPin() const
{
    return FindPinChecked(TEXT("RowName"));
}
