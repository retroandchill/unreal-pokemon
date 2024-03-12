// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DataManager.h"
#include "DataRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "DataUtilities.generated.h"

/**
 * Utility library used for getting data for the custom nodes
 */
UCLASS(Blueprintable)
class POKEMONDATA_API UDataUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Get a data item from the Database
	 * @param StructType The struct type to get the data from
	 * @param RowName The name of the row to retrieve
	 * @param OutRow The output struct returned to the user
	 */
	UFUNCTION(BlueprintPure, CustomThunk, Category = "Data Table",
		meta=(CustomStructureParam = "OutRow", BlueprintInternalUseOnly="true"))
	static void GetData(const UScriptStruct* StructType, FName RowName, FTableRowBase& OutRow);

	/**
	 * Get a data item from the Database
	 * @tparam T The data type of the struct being processed out
	 * @param StructType The struct type to get the data from
	 * @param RowName The name of the row to retrieve
	 * @param OutRow The output struct returned to the user
	 */
	template <typename T>
	static void Generic_GetData(const UScriptStruct* StructType, FName RowName, T* OutRow) {
		check(StructType != nullptr && OutRow != nullptr)

		const auto Row = FDataManager::GetInstance().GetDataTable(StructType).GetData(RowName);
		StructType->CopyScriptStruct(OutRow, Row);
	}

	/**
	 * Get a data item from the Database. Based on UDataUtilities::GetData
	 * @param Context The context that this node is called from
	 * @param Stack The current state of the Blueprint callstack
	 * @param Z_Param__Result The block of memory to save the result of the execution to
	 */
	DECLARE_FUNCTION(execGetData) {
		P_GET_OBJECT(UScriptStruct, StructType);
		P_GET_PROPERTY(FNameProperty, RowName);

		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		if (const auto StructProp = CastField<FStructProperty>(Stack.MostRecentProperty); StructProp && OutRowPtr) {
			if (auto OutputType = StructProp->Struct; (OutputType == StructType) ||
				(OutputType->IsChildOf(StructType) && FStructUtils::TheSameLayout(OutputType, StructType))) {
				P_NATIVE_BEGIN;
					Generic_GetData(StructType, RowName, OutRowPtr);
				P_NATIVE_END;
			} else {
				FBlueprintExceptionInfo ExceptionInfo(
					EBlueprintExceptionType::AccessViolation,
					NSLOCTEXT("GetData", "IncompatibleProperty",
					          "Incompatible output parameter; the data table's type is not the same as the return type.")
				);
				FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
			}
		} else {
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("GetData", "MissingOutputProperty",
				          "Failed to resolve the output parameter for GetDataTableRow.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
	}


	/**
	 * Get all IDs for the particular data type
	 * @param ContextObject The context object used to retrieve the data subsystem
	 * @param StructType The struct type to get the data from
	 * @return The IDs used to lookup each row in the table
	 */
	UFUNCTION(BlueprintPure, Category = "Data Table",
		meta=(WorldContext = "ContextObject", BlueprintInternalUseOnly="true"))
	static TSet<FName> GetAllDataIDs(const UScriptStruct* StructType);

	/**
	 * Check if the given row name in the table exists
	 * @param ContextObject The context object used to retrieve the data subsystem
	 * @param StructType The struct type to get the data from
	 * @param RowName The name of the row to retrieve
	 * @return Does the row name in question exists?
	 */
	UFUNCTION(BlueprintPure, Category = "Data Table",
		meta=(WorldContext = "ContextObject", BlueprintInternalUseOnly="true"))
	static bool IsDataRowNameValid(UObject* ContextObject, const UScriptStruct* StructType, FName RowName);

	/**
	 * Utility method used by the K2Nodes to add all the types to the menu as needed
	 * @tparam T The type of the node we're registering
	 * @param ActionKey The class reference for the node in question
	 * @param ActionRegistrar The ActionRegistrar object passed to GetMenuActions
	 */
	template <typename T>
	static void AddAllDataTableTypesToMenu(UClass* ActionKey, FBlueprintActionDatabaseRegistrar& ActionRegistrar) {
		auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		AssetRegistryModule.Get().GetAssetsByClass(FTopLevelAssetPath(UDataTable::StaticClass()->GetPathName()),
		                                           AssetData);

		auto CustomizeCallback = [](UEdGraphNode* Node, [[maybe_unused]] bool bIsTemplateNode,
		                            const UScriptStruct* Subclass) {
			auto TypedNode = CastChecked<T>(Node);
			TypedNode->Initialize(Subclass);
		};

		if (ActionRegistrar.IsOpenForRegistration(ActionKey)) {
			auto& DataRegistry = FDataRegistry::GetInstance();
			for (auto& Iter : AssetData) {
				auto Table = Cast<UDataTable>(Iter.GetAsset());
				if (Table == nullptr)
					continue;

				auto Type = Table->GetRowStruct();
				if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Type, true) || !DataRegistry.
					IsTypeRegistered(Type))
					continue;

				UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(ActionKey);
				check(Spawner)

				Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
					CustomizeCallback, Type);
				ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
			}
		}
	}
};
