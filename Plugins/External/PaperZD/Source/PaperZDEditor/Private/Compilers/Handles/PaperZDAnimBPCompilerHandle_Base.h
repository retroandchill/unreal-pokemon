// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Compilers/Handles/IPaperZDAnimBPCompilerHandle.h"

class FPaperZDAnimBPGeneratedClassAccess;
class UPaperZDAnimGraphNode_Base;
class UK2Node;
class UEdGraphPin;
struct FPaperZDExposedValueHandler;

/**
 * Base compiler handle for every animation node, manages base compilation of variables and linkage.
 */
class FPaperZDAnimBPCompilerHandle_Base : public IPaperZDAnimBPCompilerHandle, public TUniqueClassIdentifier<FPaperZDAnimBPCompilerHandle_Base>
{
	// Constant property to be copied into the CDO
	struct FEffectiveConstantRecord
	{
		// The node variable that the handler is in
		class FStructProperty* NodeVariableProperty;

		// The property within the struct to set
		class FProperty* ConstantProperty;

		// The array index if ConstantProperty is an array property, or INDEX_NONE otherwise
		int32 ArrayIndex;

		// The pin to pull the DefaultValue/DefaultObject from
		UEdGraphPin* LiteralSourcePin;

		FEffectiveConstantRecord()
			: NodeVariableProperty(nullptr)
			, ConstantProperty(nullptr)
			, ArrayIndex(INDEX_NONE)
			, LiteralSourcePin(nullptr)
		{
		}

		FEffectiveConstantRecord(FStructProperty* ContainingNodeProperty, UEdGraphPin* SourcePin, FProperty* SourcePinProperty, int32 SourceArrayIndex)
			: NodeVariableProperty(ContainingNodeProperty)
			, ConstantProperty(SourcePinProperty)
			, ArrayIndex(SourceArrayIndex)
			, LiteralSourcePin(SourcePin)
		{
		}

		//Apply the record onto the given object
		bool Apply(UObject* Object);
	};

	/** Record of a single copy operation */
	struct FPropertyCopyRecord
	{
		/** The destination pin we are copying to */
		UEdGraphPin* DestPin;

		/** The destination property we are copying to (on an animation node) */
		FProperty* DestProperty;

		/** The array index we use if the destination property is an array */
		int32 DestArrayIndex;

		/** The property path relative to the class */
		TArray<FString> SourcePropertyPath;

		/** The property path relative to the class */
		TArray<FString> DestPropertyPath;

		//@NOTE: No FAST-PATH support for now
// 		/** The index of the copy in the property access library */
// 		int32 LibraryCopyIndex;
// 
// 		/** the batch type within the property access library */
// 		EPropertyAccessBatchType LibraryBatchType;
// 
// 		/** Any operation we want to perform post-copy on the destination data */
// 		EPostCopyOperation Operation;
// 
// 		/** Fast-path flag */
// 		bool bIsFastPath;

		FPropertyCopyRecord(UEdGraphPin* InDestPin, FProperty* InDestProperty, int32 InDestArrayIndex, TArray<FString>&& InDestPropertyPath)
			: DestPin(InDestPin)
			, DestProperty(InDestProperty)
			, DestArrayIndex(InDestArrayIndex)
			, DestPropertyPath(MoveTemp(InDestPropertyPath))
// 			, LibraryCopyIndex(INDEX_NONE)
// 			, LibraryBatchType(EPropertyAccessBatchType::Unbatched)
// 			, Operation(EPostCopyOperation::None)
// 			, bIsFastPath(true)
		{}

		FPropertyCopyRecord(const TArray<FString>& InSourcePropertyPath, const TArray<FString>& InDestPropertyPath)
			: DestPin(nullptr)
			, DestProperty(nullptr)
			, DestArrayIndex(INDEX_NONE)
			, SourcePropertyPath(InSourcePropertyPath)
			, DestPropertyPath(InDestPropertyPath)
// 			, LibraryCopyIndex(INDEX_NONE)
// 			, LibraryBatchType(EPropertyAccessBatchType::Unbatched)
// 			, Operation(EPostCopyOperation::None)
// 			, bIsFastPath(true)
		{}

		//@NOTE: No fast-path support
// 		bool IsFastPath() const
// 		{
// 			return SourcePropertyPath.Num() > 0 && bIsFastPath;
// 		}
// 
// 		void InvalidateFastPath()
// 		{
// 			bIsFastPath = false;
// 		}
	};

	// Wireup record for a single anim node property (which might be an array)
	struct FAnimNodeSinglePropertyHandler
	{
		/** Copy records */
		TArray<FPropertyCopyRecord> CopyRecords;

		// If the anim instance is the container target instead of the node.
		bool bInstanceIsTarget;

		FAnimNodeSinglePropertyHandler()
			: bInstanceIsTarget(false)
		{
		}
	};

	// Execution handler for Anim node
	struct FEvaluationHandlerRecord
	{
		// The Node this record came from
		UPaperZDAnimGraphNode_Base* AnimGraphNode;

		// The node variable that the handler is in
		FStructProperty* NodeVariableProperty;

		// The specific evaluation handler inside the specified node
		int32 EvaluationHandlerIdx;

		// Set of properties serviced by this handler (Map from property name to the record for that property)
		TMap<FName, FAnimNodeSinglePropertyHandler> ServicedProperties;

		// Name of the handler function that services this record
		FName HandlerFunctionName;

		//List of nodes used for creating the event handlers (custom events and struct setters)
		TArray<UK2Node*> CustomEventNodes;

	public:
		//ctor
		FEvaluationHandlerRecord()
		: AnimGraphNode(nullptr)
		, NodeVariableProperty(nullptr)
		, EvaluationHandlerIdx(INDEX_NONE)
		, HandlerFunctionName(NAME_None)
		{}

		//Register this evaluation handler via the given pin
		void RegisterPin(UEdGraphPin* DestPin, FProperty* AssociatedProperty, int32 AssociatedPropertyArrayIndex);

		//Copies the function's name into the exposed value handler
		void SetupExposedHandler(FPaperZDExposedValueHandler& Handler) const;
	};

	// Records of pose pins for later patchup with an associated evaluation handler
	TMap<UPaperZDAnimGraphNode_Base*, FEvaluationHandlerRecord> PerNodeStructEvalHandlers;

	// List of successfully created evaluation handlers
	TArray<FEvaluationHandlerRecord> ValidEvaluationHandlerList;
	TMap<UPaperZDAnimGraphNode_Base*, int32> ValidEvaluationHandlerMap;

	// Constant values to be patched
	TArray<FEffectiveConstantRecord> ValidAnimNodePinConstants;

	// List of existing handler names (for evaluating exposed pin data)
	TSet<FName> HandlerFunctionNames;

public:
	~FPaperZDAnimBPCompilerHandle_Base() {}

	//~Begin IPaperZDAnimBPCompilerHandle Interface
	virtual void Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess) override;
	//~End IPaperZDAnimBPCompilerHandle Interface

	/* Adds a map of struct eval handlers for the specified node */
	void AddStructEvalHandlers(UPaperZDAnimGraphNode_Base* InNode, FPaperZDAnimBPCompilerAccess& InCompilerContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Create an 'expanded' evaluation handler for the specified node, called in the compiler's node expansion step */
	void CreateEvaluationHandlerForNode(FPaperZDAnimBPCompilerAccess& InCompilationContext, UPaperZDAnimGraphNode_Base* InNode);

	/* Creates an internal "event" that's connected to a specific "pin" evaluation record. It will run the K2 logic and obtain the data that was connected to the pin. */
	void CreateEvaluationHandler(FPaperZDAnimBPCompilerAccess& InCompilationContext, UPaperZDAnimGraphNode_Base* InNode, FEvaluationHandlerRecord& Record);

private:
 	void FinishCompilingClass(const UClass* InClass, FPaperZDAnimBPCompilerAccess& InCompilerContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
 	void CopyTermDefaultsToDefaultObject(UObject* InDefaultObject, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
};

//Factory definition
typedef TPaperZDCompilerHandleFactory<FPaperZDAnimBPCompilerHandle_Base> FPaperZDCompilerFactory_Base;
