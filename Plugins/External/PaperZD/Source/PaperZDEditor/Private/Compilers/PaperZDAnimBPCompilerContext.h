// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "KismetCompiler.h"
#include "Templates/UniquePtr.h"
#include "Compilers/Handles/IPaperZDAnimBPCompilerHandle.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"

//Forward declarations
class UPaperZDAnimBP;
class UPaperZDAnimGraphNode_Base;
class UPaperZDAnimBPGeneratedClass;
class FPaperZDAnimBPCompilerAccess;
class FPaperZDAnimBPGeneratedClassAccess;
class UEdGraphSchema;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCompileClassSignature, const UClass*, FPaperZDAnimBPCompilerAccess&, FPaperZDAnimBPGeneratedClassAccess&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPostExpansionStepSignature, const UEdGraph*, FPaperZDAnimBPCompilerAccess&, FPaperZDAnimBPGeneratedClassAccess&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCopyTermDefaultsSignature, UObject*, FPaperZDAnimBPCompilerAccess&, FPaperZDAnimBPGeneratedClassAccess&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnProcessAnimationNodesSignature, TArrayView<UPaperZDAnimGraphNode_Base*> /*InAnimNodes*/, FPaperZDAnimBPCompilerAccess& /*InCompilerAccess*/, FPaperZDAnimBPGeneratedClassAccess& /*OutCompiledData*/);

/**
 * Custom compiler for the PaperZD Animation Blueprint.
 */
class FPaperZDAnimBPCompilerContext : public FKismetCompilerContext
{
	//Compiler access
	friend class FPaperZDAnimBPCompilerAccess;

	/* Delegates to each section of the compilation process. Used by the Handles to inject code. */
	FOnCompileClassSignature OnStartCompilingClass;
	FOnCompileClassSignature OnFinishCompilingClass;
	FOnPostExpansionStepSignature OnPostExpansionStep;
	FOnCopyTermDefaultsSignature OnCopyTermDefaultsToDefaultObject;
	FOnProcessAnimationNodesSignature OnPreProcessAnimationNodes;
	FOnProcessAnimationNodesSignature OnPostProcessAnimationNodes;

	/* Animation Blueprint being compiled. */
	UPaperZDAnimBP* AnimBP = nullptr;
	UPaperZDAnimBPGeneratedClass* NewAnimBlueprintClass = nullptr;

	// Map of allocated v3 nodes that are members of the class
	TMap<UPaperZDAnimGraphNode_Base*, FProperty*> AllocatedAnimNodes;
	TMap<FProperty*, UPaperZDAnimGraphNode_Base*> AllocatedNodePropertiesToNodes;
	TMap<int32, FProperty*> AllocatedPropertiesByIndex;

	// Map of true source objects (user edited ones) to the cloned ones that are actually compiled
	TMap<UPaperZDAnimGraphNode_Base*, UPaperZDAnimGraphNode_Base*> SourceNodeToProcessedNodeMap;

	// Index of the nodes (must match up with the runtime discovery process of nodes, which runs thru the property chain)
	int32 AllocateNodeIndexCounter;
	TMap<UPaperZDAnimGraphNode_Base*, int32> AllocatedAnimNodeIndices;

	// Per instance compile handlers
	TMap<zid_t, TUniquePtr<IPaperZDAnimBPCompilerHandle>> CompileHandles;

	//List of compiler factories registered globally
	static TMap<zid_t, TUniquePtr<IPaperZDCompilerHandleFactory>> RegisteredHandleFactories;

	// List of graph schema classes that are managed by this compiler (and should not be processed as functions by the kismet compiler)
	TArray<TSubclassOf<UEdGraphSchema>> KnownGraphSchemas;

	//AnimData LinkRecords for later patch-up
	TArray<FPaperZDAnimDataLinkRecord> LinkRecords;

public:
	//ctor
	FPaperZDAnimBPCompilerContext(UBlueprint* Blueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions);

	/* Request compile-time access. Will fail if the given compiler is not a ZD compiler. */
	static TUniquePtr<FPaperZDAnimBPCompilerAccess> RequestAccess(FKismetCompilerContext& CompilerContext);

	/* Request the registration of the given compiler handle factory class. */
	template <typename T>
	static void RegisterCompileHandleFactory()
	{
		//Only one register per class
 		if (!RegisteredHandleFactories.Contains(T::GetClassId()))
		{
 			IPaperZDCompilerHandleFactory* NewFactory = new T();
			RegisteredHandleFactories.Add(T::GetClassId(), TUniquePtr<IPaperZDCompilerHandleFactory>(NewFactory));
		}
	}

	/* Unregisters a compiler handle factory class. */
	template <typename T>
	static void UnregisterCompileHandleFactory()
	{
		RegisteredHandleFactories.Remove(T::GetClassId());
	}

protected:
	//~Begin FKismetCompilerContext Interface
 	virtual UEdGraphSchema_K2* CreateSchema() override;
	virtual void CreateFunctionList() override;
 	virtual void MergeUbergraphPagesIn(UEdGraph* Ubergraph) override;
 	virtual void ProcessOneFunctionGraph(UEdGraph* SourceGraph, bool bInternalFunction = false) override;
 	virtual void SpawnNewClass(const FString& NewClassName) override;
 	virtual void OnNewClassSet(UBlueprintGeneratedClass* ClassToUse) override;
 	virtual void OnPostCDOCompiled(const UObject::FPostCDOCompiledContext& Context) override;
 	virtual void CopyTermDefaultsToDefaultObject(UObject* DefaultObject) override;
	virtual void PreCompile() override;
 	virtual void PostCompile() override;
// 	virtual void PostCompileDiagnostics() override;
 	virtual void EnsureProperGeneratedClass(UClass*& TargetClass) override;
 	virtual void CleanAndSanitizeClass(UBlueprintGeneratedClass* ClassToClean, UObject*& InOldCDO) override;
 	virtual void FinishCompilingClass(UClass* Class) override;
// 	virtual void PrecompileFunction(FKismetFunctionContext& Context, EInternalCompilerFlags InternalFlags) override;
// 	virtual void SetCalculatedMetaDataAndFlags(UFunction* Function, UK2Node_FunctionEntry* EntryNode, const UEdGraphSchema_K2* Schema) override;
 	virtual bool ShouldForceKeepNode(const UEdGraphNode* Node) const override;
 	virtual void PostExpansionStep(const UEdGraph* Graph) override;
	//~End FKismetCompilerContext Interface

private:
	// Run a function on the passed-in graph and each subgraph of it
	void ForAllSubGraphs(UEdGraph* InGraph, TFunctionRef<void(UEdGraph*)> InPerGraphFunction);

	// Prunes any nodes that aren't reachable via an AnimData link
	void PruneIsolatedAnimationNodes(const TArray<UPaperZDAnimGraphNode_Base*>& RootSet, TArray<UPaperZDAnimGraphNode_Base*>& GraphNodes);

	/* Processes all the supplied animation nodes. */
	void ProcessAllAnimationNodes();
	
	/* Processes all the supplied animation nodes. */
	void ProcessAnimationNodes(TArray<UPaperZDAnimGraphNode_Base*>& AnimNodeList);

	/* Compiles one animation node */
	void ProcessAnimationNode(UPaperZDAnimGraphNode_Base* VisualAnimNode);

	/* Expand split pins for a graph. */
	void ExpandSplitPins(UEdGraph* Graph);

	/* Updates the AnimNotify function list. */
	void UpdateAnimNotifyFunctions();

	/* Gets all anim graph nodes that are piped into the provided node (traverses input pins). */
	void GetLinkedAnimNodes(UPaperZDAnimGraphNode_Base* InGraphNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const;
	void GetLinkedAnimNodes_TraversePin(UEdGraphPin* InPin, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const;
	void GetLinkedAnimNodes_ProcessAnimNode(UPaperZDAnimGraphNode_Base* AnimNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const;
};
