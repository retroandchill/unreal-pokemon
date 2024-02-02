// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataUtilities.generated.h"

/**
 * Utility library used for getting data for the custom nodes
 */
UCLASS()
class POKEMONDATA_API UDataUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Get a data item from the Database
	 * @param ContextObject The context object used to retrieve the data subsystem
	 * @param StructType The struct type to get the data from
	 * @param RowName The name of the row to retrieve
	 * @param OutRow The output struct returned to the user
	 */
	UFUNCTION(BlueprintPure, CustomThunk, Category = "DataTable", meta=(CustomStructureParam = "OutRow", BlueprintInternalUseOnly="true"))
	static void GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName, FTableRowBase& OutRow);

	/**
	 * Get a data item from the Database
	 * @param ContextObject The context object used to retrieve the data subsystem
	 * @param StructType The struct type to get the data from
	 * @param RowName The name of the row to retrieve
	 * @param OutRow The output struct returned to the user
	 */
	static void Generic_GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName, void* OutRow);

	/** Based on UDataUtilities::GetData */
	DECLARE_FUNCTION(execGetData)
	{
		P_GET_OBJECT(UObject, ContextObject);
		P_GET_OBJECT(UScriptStruct, StructType);
		P_GET_PROPERTY(FNameProperty, RowName);
        
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* OutRowPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		
		FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
		if (!ContextObject)
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("GetData", "MissingWorldContext", "Failed to world context object. Unable to retrieve subsystems.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
		else if(StructProp && OutRowPtr)
		{
			UScriptStruct* OutputType = StructProp->Struct;
		
			const bool bCompatible = (OutputType == StructType) || 
				(OutputType->IsChildOf(StructType) && FStructUtils::TheSameLayout(OutputType, StructType));
			if (bCompatible)
			{
				P_NATIVE_BEGIN;
				Generic_GetData(ContextObject, StructType, RowName, OutRowPtr);
				P_NATIVE_END;
			}
			else
			{
				FBlueprintExceptionInfo ExceptionInfo(
					EBlueprintExceptionType::AccessViolation,
					NSLOCTEXT("GetData", "IncompatibleProperty", "Incompatible output parameter; the data table's type is not the same as the return type.")
					);
				FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
			}
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("GetData", "MissingOutputProperty", "Failed to resolve the output parameter for GetDataTableRow.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
	}
};
