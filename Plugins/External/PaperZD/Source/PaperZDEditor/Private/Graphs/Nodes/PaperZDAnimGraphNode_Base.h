// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node.h"
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimBP.h"
#include "PaperZDAnimGraphNode_Base.generated.h"

class IDetailLayoutBuilder;
class FCompilerResultsLog;
class FPaperZDAnimBPCompilerAccess;
class FPaperZDAnimBPGeneratedClassAccess;
class UPaperZDAnimGraphNode_Base;
class UPaperZDAnimGraph;

/**
 * Records an AnimData connection between two animation nodes, used for post-compilation linking
 */
class FPaperZDAnimDataLinkRecord
{
	// Linked node for this pose link, can be nullptr
	TObjectPtr<UPaperZDAnimGraphNode_Base> LinkedNode;

	// Linking node for this pose link, can be nullptr
	TObjectPtr<UPaperZDAnimGraphNode_Base> LinkingNode;

	// Will either be an array property containing FPaperZDAnimDataLink derived structs, indexed by ChildPropertyIndex, or a FPaperZDAnimDataLink derived struct property 
	FProperty* ChildProperty;

	// Index when ChildProperty is an array
	int32 ChildPropertyIndex;

	//private ctor, can only be made via "MAKE~" methods
	FPaperZDAnimDataLinkRecord()
	 : LinkedNode(nullptr)
	 , LinkingNode(nullptr)
	 , ChildProperty(nullptr)
	 , ChildPropertyIndex(INDEX_NONE)
	{}

public:
	static FPaperZDAnimDataLinkRecord MakeFromArrayEntry(UPaperZDAnimGraphNode_Base* LinkingNode, UPaperZDAnimGraphNode_Base* LinkedNode, FArrayProperty* ArrayProperty, int32 ArrayIndex)
	{
		checkSlow(CastFieldChecked<FStructProperty>(ArrayProperty->Inner)->Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()));

		FPaperZDAnimDataLinkRecord Result;
		Result.LinkingNode = LinkingNode;
		Result.LinkedNode = LinkedNode;
		Result.ChildProperty = ArrayProperty;
		Result.ChildPropertyIndex = ArrayIndex;

		return Result;
	}

	static FPaperZDAnimDataLinkRecord MakeFromMember(UPaperZDAnimGraphNode_Base* LinkingNode, UPaperZDAnimGraphNode_Base* LinkedNode, FStructProperty* MemberProperty)
	{
		checkSlow(MemberProperty->Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()));

		FPaperZDAnimDataLinkRecord Result;
		Result.LinkingNode = LinkingNode;
		Result.LinkedNode = LinkedNode;
		Result.ChildProperty = MemberProperty;
		Result.ChildPropertyIndex = INDEX_NONE;

		return Result;
	}

	static FPaperZDAnimDataLinkRecord MakeInvalid()
	{
		return FPaperZDAnimDataLinkRecord();
	}

	/* Get the linking node. */
	FORCEINLINE UPaperZDAnimGraphNode_Base* GetLinkingNode() const
	{
		return LinkingNode;
	}

	/* Get the linked node. */
	FORCEINLINE UPaperZDAnimGraphNode_Base* GetLinkedNode() const
	{
		return LinkedNode;
	}

	/* Is this a valid link?. */
	FORCEINLINE bool IsValid() const
	{
		return LinkedNode && LinkingNode && ChildProperty;
	}

	/* Patches the link index onto the destination ptr. */
	void PatchLinkIndex(uint8* DestinationPtr, int32 LinkID, int32 SourceLinkID) const;
};

/**
 * Base class for all the "Animation Graph" nodes for PaperZD.
 * Must be paired with a Runtime node implementation.
 */
UCLASS(abstract)
class UPaperZDAnimGraphNode_Base : public UK2Node
{
	GENERATED_BODY()

	/* Friendship for managing ShowPinForProperties directly. */
	friend class FPaperZDAnimGraphNodeBindingExtension;

	/* List of cached pins to show. */
	UPROPERTY(EditAnywhere, Category = PinOptions, EditFixedSize)
	TArray<FOptionalPinFromProperty> ShowPinForProperties;

	/* Old pins. */
	TArray<FName> OldShownPins;

public:
	UPaperZDAnimGraphNode_Base();

	// UObject interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	//virtual void Serialize(FArchive& Ar) override;
	// End of UObject interface

	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool ShowPaletteIconOnNode() const override { return false; }
//	virtual FString GetDocumentationLink() const override;
// 	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;
// 	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
// 	virtual FString GetPinMetaData(FName InPinName, FName InKey) override;
// 	virtual void AddSearchMetaDataInfo(TArray<struct FSearchTagDataPair>& OutTaggedMetaData) const override;
// 	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	// End of UEdGraphNode interface

	// UK2Node interface
 	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
 	virtual bool ShouldShowNodeProperties() const override { return true; }
 	virtual bool CanPlaceBreakpoints() const override { return false; }
 	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
 	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;
 	virtual void GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const override;
 	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
 	virtual FText GetMenuCategory() const override;
 	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// End UK2Node Interface

	// Gets the menu category this node belongs in
	virtual FString GetNodeCategory() const;

	// Gives each visual node a chance to do final validation before it's node is harvested for use at runtime
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog) {}

	// Gives each visual node a chance to validate that they are still valid in the context of the compiled class, giving a last shot at error or warning generation after primary compilation is finished
	//virtual void ValidateAnimNodePostCompile(FCompilerResultsLog& MessageLog, UAnimBlueprintGeneratedClass* CompiledClass, int32 CompiledNodeIndex) {}

	// Process this node's data during compilation
	void ProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
	virtual void OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData) {}

	// Gives each visual node a chance to update the node template before it is inserted in the compiled class
	virtual void BakeDataDuringCompilation(FCompilerResultsLog& MessageLog) {}

	// Get the property (and possibly array index) associated with the supplied pin
	virtual void GetPinAssociatedProperty(const UScriptStruct* NodeType, const UEdGraphPin* InputPin, FProperty*& OutProperty, int32& OutIndex) const;

	/* Checks if the given property feature is active on this node. */
	virtual bool IsPropertyFeatureActive(FName FeatureName) const;

	/* Checks if this node is compatible with the given Animation Source (source wide features). */
	virtual bool IsCompatibleWithAnimationSource(UPaperZDAnimationSource* InAnimSource) const;

	// Get the AnimDataLinkRecord that represents the given input pin
	virtual FPaperZDAnimDataLinkRecord GetLinkIDLocation(const UScriptStruct* NodeType, UEdGraphPin* InputLinkPin);

	/** Get the animation blueprint to which this node belongs */
	UPaperZDAnimBP* GetAnimBlueprint() const { return CastChecked<UPaperZDAnimBP>(GetBlueprint()); }

	/**
	 * Get the animation graph to which this node belongs. 
	 * The AnimGraph can be null during compilation if this node is part of the top-most animation graph, as it gets copied into the consolidated event graph.
	 */
	UPaperZDAnimGraph* GetAnimGraph() const;

protected:
	friend class FPaperZDAnimGraphNodeDetailCustomization;
	friend class FPaperZDAnimBPCompilerContext;
	friend class FPaperZDAnimBPCompilerHandle_Base;

	/* Creates the output pin, if required. */
	virtual void CreateOutputPin();

	/* True if we want to create output pins. */
	virtual bool RequiresOutputPin() const;

	/* Gets the animation FNode type represented by this ed graph node. */
	UScriptStruct* GetFNodeType() const;

	/* Gets the animation FNode property represented by this ed graph node. */
	FStructProperty* GetFNodeProperty() const;

	/* Can customize details tab */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) { }

	/* Chance to customize the pin on creation. */
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const {}

	/* Shows or hides the pin for the given property. */
	virtual void SetShowPinForProperty(FName PropertyName, bool bShowPin);

private:
	// Allocates or reallocates pins
	void InternalPinCreation(TArray<UEdGraphPin*>* OldPins);
};
