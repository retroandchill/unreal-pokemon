// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Graphs/PaperZDAnimGraph.h"
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Compilers/Handles/PaperZDAnimBPCompilerHandle_Base.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_Base)
#endif


namespace FPaperZDAnimGraphNodeUtils
{
	//Common pin names
	FName NAME_NeverAsPin = TEXT("NeverAsPin");
	FName NAME_PinHiddenByDefault = TEXT("HidePinByDefault");
	FName NAME_PinShownByDefault = TEXT("ShowPinByDefault");
	FName NAME_AlwaysAsPin = TEXT("AlwaysAsPin");
	FName NAME_CustomizeProperty = TEXT("CustomizeProperty");
	FName NAME_PropertyFeature_NonTransitional = TEXT("NonTransitional");
	FName NAME_PropertyFeature_TransitionalOnly = TEXT("TransitionalOnly");
}

//////////////////////////////////////////////////////////////////////////
// Data Link Record
//////////////////////////////////////////////////////////////////////////
void FPaperZDAnimDataLinkRecord::PatchLinkIndex(uint8* DestinationPtr, int32 LinkID, int32 SourceLinkID) const
{
	checkSlow(IsValid());

	//Obtain the pointer to the property that has the "link"
	uint8* Property = ChildProperty->ContainerPtrToValuePtr<uint8>(DestinationPtr);

	//check for array behavior
	if (ChildPropertyIndex != INDEX_NONE)
	{	
		//Grab the raw ptr
		FArrayProperty* ArrayProp = CastFieldChecked<FArrayProperty>(ChildProperty);
		FScriptArrayHelper Helper(ArrayProp, Property);
		Property = Helper.GetRawPtr(ChildPropertyIndex);
	}

	// Check to guard against accidental infinite loops
	check((LinkID == INDEX_NONE) || (LinkID != SourceLinkID));

	//Pass the data
	FPaperZDAnimDataLink* DataLink = reinterpret_cast<FPaperZDAnimDataLink*>(Property);
	DataLink->LinkID = LinkID;
	DataLink->SourceLinkID = SourceLinkID;
}

//////////////////////////////////////////////////////////////////////////
// Graph node base
//////////////////////////////////////////////////////////////////////////

UPaperZDAnimGraphNode_Base::UPaperZDAnimGraphNode_Base()
	: Super()
{}

void UPaperZDAnimGraphNode_Base::PreEditChange(FProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);

	if (PropertyAboutToChange && PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin))
	{
		FOptionalPinManager::CacheShownPins(ShowPinForProperties, OldShownPins);
	}
}

void UPaperZDAnimGraphNode_Base::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if ((PropertyName == GET_MEMBER_NAME_CHECKED(FOptionalPinFromProperty, bShowPin)))
	{
		FOptionalPinManager::EvaluateOldShownPins(ShowPinForProperties, OldShownPins, this);
		GetSchema()->ReconstructNode(*this);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);

	//PropertyChangeEvent.Broadcast(PropertyChangedEvent);
}

void UPaperZDAnimGraphNode_Base::AllocateDefaultPins()
{
	InternalPinCreation(nullptr);
}

FLinearColor UPaperZDAnimGraphNode_Base::GetNodeTitleColor() const
{
	return FLinearColor::Black;
}

void UPaperZDAnimGraphNode_Base::CreateOutputPin()
{
	if (RequiresOutputPin())
	{
		CreatePin(EGPD_Output, UPaperZDAnimGraphSchema::PC_Struct, FPaperZDAnimDataLink::StaticStruct(), TEXT("Animation"));	
	}
}

bool UPaperZDAnimGraphNode_Base::RequiresOutputPin() const
{
	return true;
}

UScriptStruct* UPaperZDAnimGraphNode_Base::GetFNodeType() const
{
	UScriptStruct* BaseFStruct = FPaperZDAnimNode_Base::StaticStruct();

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct->IsChildOf(BaseFStruct))
			{
				return StructProp->Struct;
			}
		}
	}

	return NULL;
}

FStructProperty* UPaperZDAnimGraphNode_Base::GetFNodeProperty() const
{
	UScriptStruct* BaseFStruct = FPaperZDAnimNode_Base::StaticStruct();

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		if (FStructProperty* StructProp = CastField<FStructProperty>(*PropIt))
		{
			if (StructProp->Struct->IsChildOf(BaseFStruct))
			{
				return StructProp;
			}
		}
	}

	return NULL;
}

void UPaperZDAnimGraphNode_Base::SetShowPinForProperty(FName PropertyName, bool bShowPin)
{
	for (FOptionalPinFromProperty& OptionalPinProperty : ShowPinForProperties)
	{
		if (OptionalPinProperty.PropertyName == PropertyName)
		{
			OptionalPinProperty.bShowPin = bShowPin;
			GetSchema()->ReconstructNode(*this);
			break;
		}
	}
}

void UPaperZDAnimGraphNode_Base::InternalPinCreation(TArray<UEdGraphPin*>* OldPins)
{
	// Chance to preload required assets first before creating pins
	PreloadRequiredAssets();

	//Custom implementation of the pin manager
	struct FPaperZDAnimPinManager : public FOptionalPinManager
	{
		// Old pin relations
		UPaperZDAnimGraphNode_Base* BaseNode;
		TArray<UEdGraphPin*>* OldPins;
		TMap<FName, UEdGraphPin*> OldPinMap;

		FPaperZDAnimPinManager(UPaperZDAnimGraphNode_Base* InBaseNode, TArray<UEdGraphPin*>* InOldPins)
			: BaseNode(InBaseNode)
			, OldPins(InOldPins)
		{
			if (OldPins)
			{
				for (UEdGraphPin* Pin : *OldPins)
				{
					OldPinMap.Add(Pin->PinName, Pin);
				}
			}
		}

		// FOptionalPinsUpdater interface
		virtual void GetRecordDefaults(FProperty* TestProperty, FOptionalPinFromProperty& Record) const override
		{
			// Determine if this is a pose or array of poses
			FArrayProperty* ArrayProp = CastField<FArrayProperty>(TestProperty);
			FStructProperty* StructProp = CastField<FStructProperty>(ArrayProp ? ArrayProp->Inner : TestProperty);
			const bool bIsAnimLinkInput = (StructProp && StructProp->Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()));

			//@TODO: Error if they specified two or more of these flags
			const bool bAlwaysShow = TestProperty->HasMetaData(FPaperZDAnimGraphNodeUtils::NAME_AlwaysAsPin) || bIsAnimLinkInput;
			const bool bOptional_ShowByDefault = TestProperty->HasMetaData(FPaperZDAnimGraphNodeUtils::NAME_PinShownByDefault);
			const bool bOptional_HideByDefault = TestProperty->HasMetaData(FPaperZDAnimGraphNodeUtils::NAME_PinHiddenByDefault);
			const bool bNeverShow = TestProperty->HasMetaData(FPaperZDAnimGraphNodeUtils::NAME_NeverAsPin);
			const bool bCanTreatPropertyAsOptional = CanTreatPropertyAsOptional(TestProperty);
			const bool bPropertyIsCustomized = TestProperty->HasMetaData(FPaperZDAnimGraphNodeUtils::NAME_CustomizeProperty);

			Record.bCanToggleVisibility = bCanTreatPropertyAsOptional && (bOptional_ShowByDefault || bOptional_HideByDefault);
			Record.bShowPin = bAlwaysShow || bOptional_ShowByDefault;
			Record.bPropertyIsCustomized = bPropertyIsCustomized;
		}

		virtual void PostInitNewPin(UEdGraphPin* Pin, FOptionalPinFromProperty& Record, int32 ArrayIndex, FProperty* Property, uint8* PropertyAddress, uint8* DefaultPropertyAddress) const
		{
			check(PropertyAddress != nullptr);
			check(Record.bShowPin);

			const UPaperZDAnimGraphSchema* Schema = GetDefault<UPaperZDAnimGraphSchema>();

			//Start by setting the default values, if they exist
			if (DefaultPropertyAddress != nullptr)
			{
				FString LiteralValue;
				FBlueprintEditorUtils::PropertyValueToString_Direct(Property, DefaultPropertyAddress, LiteralValue);
				Schema->SetPinAutogeneratedDefaultValue(Pin, LiteralValue);
			}
			else
			{
				Schema->SetPinAutogeneratedDefaultValueBasedOnType(Pin);
			}

			//Manage new pin setup
			if (OldPins == nullptr)
			{
				// Initial construction of a visible pin; copy values from the struct
				FString LiteralValue;
				FBlueprintEditorUtils::PropertyValueToString_Direct(Property, PropertyAddress, LiteralValue);

				Schema->SetPinDefaultValueAtConstruction(Pin, LiteralValue);
			}
			else if (Record.bCanToggleVisibility)
			{
				if (UEdGraphPin* OldPin = OldPinMap.FindRef(Pin->PinName))
				{
					// Was already visible, values will get copied over in pin reconstruction code
				}
				else
				{
					// Convert the struct property into DefaultValue/DefaultValueObject
					FString LiteralValue;
					FBlueprintEditorUtils::PropertyValueToString_Direct(Property, PropertyAddress, LiteralValue);

					Schema->SetPinDefaultValueAtConstruction(Pin, LiteralValue);
				}

				// Clear the asset reference on the node if the pin exists
				// In theory this is only needed for pins that are newly created but there are a lot of existing nodes that have dead asset references
				FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property);
				if (ObjectProperty)
				{
					ObjectProperty->SetObjectPropertyValue(PropertyAddress, nullptr);
				}
			}
		}

		virtual void PostRemovedOldPin(FOptionalPinFromProperty& Record, int32 ArrayIndex, FProperty* Property, uint8* PropertyAddress, uint8* DefaultPropertyAddress) const
		{
			check(PropertyAddress);
			check(!Record.bShowPin);

			//Need to make sure that any pin "literal" value that was written gets updated onto the structure
			if (Record.bCanToggleVisibility && (OldPins != nullptr))
			{
				const FName OldPinName = (ArrayIndex != INDEX_NONE) ? *FString::Printf(TEXT("%s_%d"), *(Record.PropertyName.ToString()), ArrayIndex) : Record.PropertyName;
				// Pin was visible but it's now hidden
				if (UEdGraphPin* OldPin = OldPinMap.FindRef(OldPinName))
				{
					// Convert DefaultValue/DefaultValueObject and push back into the struct
					FBlueprintEditorUtils::PropertyValueFromString_Direct(Property, OldPin->GetDefaultAsString(), PropertyAddress, OldPin->GetOwningNodeUnchecked());
				}
			}
		}

		virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex, FProperty* Property) const override
		{
			if (BaseNode)
			{
				BaseNode->CustomizePinData(Pin, SourcePropertyName, ArrayIndex);
			}
		}
		// End of FOptionalPinsUpdater interface
	};

	if (FStructProperty* StructProperty = GetFNodeProperty())
	{
		UObject* NodeDefaults = GetArchetype();

		FPaperZDAnimPinManager PinManager(this, OldPins);
		PinManager.RebuildPropertyList(ShowPinForProperties, StructProperty->Struct);
		PinManager.CreateVisiblePins(ShowPinForProperties, StructProperty->Struct, EGPD_Input, this, StructProperty->ContainerPtrToValuePtr<uint8>(this), NodeDefaults ? StructProperty->ContainerPtrToValuePtr<uint8>(NodeDefaults) : nullptr);

		//Finally create the optional output pin
		CreateOutputPin();
	}
}

void UPaperZDAnimGraphNode_Base::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	InternalPinCreation(&OldPins);
	RestoreSplitPins(OldPins);
}

bool UPaperZDAnimGraphNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UPaperZDAnimGraphSchema::StaticClass());
}

void UPaperZDAnimGraphNode_Base::GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const
{
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Type"), TEXT("ZDAnimGraphNode")));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Class"), GetClass()->GetName()));
	OutNodeAttributes.Add(TKeyValuePair<FString, FString>(TEXT("Name"), GetName()));
}

void UPaperZDAnimGraphNode_Base::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

bool UPaperZDAnimGraphNode_Base::IsActionFilteredOut(class FBlueprintActionFilter const& Filter)
{
	for (UBlueprint* Blueprint : Filter.Context.Blueprints)
	{
		UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Blueprint);
		if (AnimBP && AnimBP->GetSupportedAnimationSource())
		{
			return !IsCompatibleWithAnimationSource(AnimBP->GetSupportedAnimationSource());
		}
	}

	return true;
}

FText UPaperZDAnimGraphNode_Base::GetMenuCategory() const
{
	return FText::FromString(GetNodeCategory());
}

FString UPaperZDAnimGraphNode_Base::GetNodeCategory() const
{
	return TEXT("Misc.");
}

void UPaperZDAnimGraphNode_Base::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
 	TUniquePtr<FPaperZDAnimBPCompilerAccess> AccessPtr = FPaperZDAnimBPCompilerContext::RequestAccess(CompilerContext);
	if (AccessPtr.IsValid())
	{
		FPaperZDAnimBPCompilerHandle_Base* Handle = AccessPtr->GetHandle<FPaperZDAnimBPCompilerHandle_Base>();
		Handle->CreateEvaluationHandlerForNode(*AccessPtr.Get(), this);
	}
}

void UPaperZDAnimGraphNode_Base::ProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilerAccess, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	//Base processing here
	FPaperZDAnimBPCompilerHandle_Base* Handle = InCompilerAccess.GetHandle<FPaperZDAnimBPCompilerHandle_Base>();
	Handle->AddStructEvalHandlers(this, InCompilerAccess, OutCompiledData);

	//Chance for any child class to do their own processing
	OnProcessDuringCompilation(InCompilerAccess, OutCompiledData);
}

void UPaperZDAnimGraphNode_Base::GetPinAssociatedProperty(const UScriptStruct* NodeType, const UEdGraphPin* InputPin, FProperty*& OutProperty, int32& OutIndex) const
{
	OutProperty = nullptr;
	OutIndex = INDEX_NONE;

	//@TODO: Name-based hackery, avoid the roundtrip and better indicate when it's an array pose pin
	const FString PinNameStr = InputPin->PinName.ToString();
	const int32 UnderscoreIndex = PinNameStr.Find(TEXT("_"), ESearchCase::CaseSensitive);
	if (UnderscoreIndex != INDEX_NONE)
	{
		const FString ArrayName = PinNameStr.Left(UnderscoreIndex);

		if (FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(NodeType, *ArrayName))
		{
			const int32 ArrayIndex = FCString::Atoi(*(PinNameStr.Mid(UnderscoreIndex + 1)));

			OutProperty = ArrayProperty;
			OutIndex = ArrayIndex;
		}
	}

	// If the array check failed or we have no underscores
	if (OutProperty == nullptr)
	{
		if (FProperty* Property = FindFProperty<FProperty>(NodeType, InputPin->PinName))
		{
			OutProperty = Property;
			OutIndex = INDEX_NONE;
		}
	}
}

bool UPaperZDAnimGraphNode_Base::IsPropertyFeatureActive(FName FeatureName) const
{
	//First check the feature properties that depend on the graph we are currently on
	if (FeatureName == FPaperZDAnimGraphNodeUtils::NAME_PropertyFeature_NonTransitional)
	{
		const UPaperZDAnimGraph* AnimGraph = GetAnimGraph();
		return AnimGraph && !AnimGraph->IsTransitionalGraph();
	}
	else if (FeatureName == FPaperZDAnimGraphNodeUtils::NAME_PropertyFeature_TransitionalOnly)
	{
		const UPaperZDAnimGraph* AnimGraph = GetAnimGraph();
		return AnimGraph && AnimGraph->IsTransitionalGraph();
	}

	return false;
}

bool UPaperZDAnimGraphNode_Base::IsCompatibleWithAnimationSource(UPaperZDAnimationSource* InAnimSource) const
{
	//Default to not requiring any feature, should be overridden by classes that require something special
	return true;
}

FPaperZDAnimDataLinkRecord UPaperZDAnimGraphNode_Base::GetLinkIDLocation(const UScriptStruct* NodeType, UEdGraphPin* InputLinkPin)
{
	if (InputLinkPin->LinkedTo.Num() > 0)
	{
		if (UPaperZDAnimGraphNode_Base* LinkedNode = Cast<UPaperZDAnimGraphNode_Base>(FBlueprintEditorUtils::FindFirstCompilerRelevantNode(InputLinkPin->LinkedTo[0])))
		{
			//See if its an array pin (name based)
			const FString SourcePinName = InputLinkPin->PinName.ToString();
			const int32 UnderscoreIndex = SourcePinName.Find(TEXT("_"), ESearchCase::CaseSensitive);
			if (UnderscoreIndex != INDEX_NONE)
			{
				const FString ArrayName = SourcePinName.Left(UnderscoreIndex);
				if (FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(NodeType, *ArrayName))
				{
					if (FStructProperty* Property = CastField<FStructProperty>(ArrayProperty->Inner))
					{
						if (Property->Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()))
						{
							const int32 ArrayIndex = FCString::Atoi(*(SourcePinName.Mid(UnderscoreIndex + 1)));
							return FPaperZDAnimDataLinkRecord::MakeFromArrayEntry(this, LinkedNode, ArrayProperty, ArrayIndex);
						}
					}
				}
			}
			else
			{
				if (FStructProperty* Property = FindFProperty<FStructProperty>(NodeType, InputLinkPin->PinName))
				{
					if (Property->Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()))
					{
						return FPaperZDAnimDataLinkRecord::MakeFromMember(this, LinkedNode, Property);
					}
				}
			}
		}
	}

	return FPaperZDAnimDataLinkRecord::MakeInvalid();
}

UPaperZDAnimGraph* UPaperZDAnimGraphNode_Base::GetAnimGraph() const
{
	return Cast<UPaperZDAnimGraph>(GetGraph());
}
