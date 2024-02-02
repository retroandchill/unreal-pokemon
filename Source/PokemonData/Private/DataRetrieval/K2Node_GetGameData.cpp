// Created by Retro & Chill.


#include "DataRetrieval/K2Node_GetGameData.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UK2Node_GetGameData::Initialize(const UScriptStruct* NodeStruct) {
	StructType = NodeStruct;
}

void UK2Node_GetGameData::AllocateDefaultPins() {
	if (GetBlueprint()->ParentClass->HasMetaData(FBlueprintMetadata::MD_ShowWorldContextPin)) {
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), TEXT("WorldContext"));
	}
	
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, TEXT("RowName"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, (StructType != nullptr ? static_cast<UStruct*>(StructType) : FTableRowBase::StaticStruct()), UEdGraphSchema_K2::PN_ReturnValue);
	
	Super::AllocateDefaultPins();
}

FText UK2Node_GetGameData::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	if (StructType)
	{
		if (TitleType == ENodeTitleType::FullTitle)
		{
			return StructType->GetDisplayNameText();
		}

		const FString& StructName = StructType->GetName();
		return FText::FormatNamed(NSLOCTEXT("K2Node", "GetGameData_NodeTitleFormat", "Get {ClassName}"), TEXT("ClassName"), FText::FromString(StructName));
	}

	return GetTooltipText();
}

FText UK2Node_GetGameData::GetTooltipText() const {
	if (StructType)
	{
		return FText::FormatNamed(NSLOCTEXT("K2Node", "GetSubsystem_TooltipFormat", "Get {StructName} \n\n{StructTooltip}"),
			TEXT("StructName"), StructType->GetDisplayNameText(),
			TEXT("StructTooltip"), StructType->GetToolTipText(/*bShortTooltip=*/ true));
	}

	return NSLOCTEXT("K2Node", "GetSubsystem_InvalidSubsystemTypeTooltip", "Invalid Subsystem Type");
}

bool UK2Node_GetGameData::IsNodePure() const {
	return true;
}

void UK2Node_GetGameData::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	Super::GetMenuActions(ActionRegistrar);

	auto &AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(UDataTable::StaticClass()->GetFName(), AssetData);

	auto CustomizeCallback = [](UEdGraphNode* Node, bool bIsTemplateNode, const UScriptStruct* Subclass)
	{
		auto TypedNode = CastChecked<UK2Node_GetGameData>(Node);
		TypedNode->Initialize(Subclass);
	};

	auto ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		for (auto &Iter : AssetData)
		{
			auto Table = Cast<UDataTable>(Iter.GetAsset());
			if (Table == nullptr)
				continue;

			auto Type = Table->GetRowStruct();
			if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Type, true))
				continue;

			UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(ActionKey);
			check(Spawner);

			Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, Type);
			ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
		}
	}
}
