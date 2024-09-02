// "Unreal Pokémon" created by Retro & Chill.


#include "Data/K2Node_SwitchOnDataHandle.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DataManager.h"
#include "Battle/Status.h"

UK2Node_SwitchOnDataHandle::UK2Node_SwitchOnDataHandle(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    FunctionName = TEXT("NotEqual_HandleHandle");
    FunctionClass = UStatusEffectHelper::StaticClass();
    OrphanedPinSaveMode = ESaveOrphanPinMode::SaveNone;
}

void UK2Node_SwitchOnDataHandle::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    auto PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    bool bIsDirty = PropertyName == GET_MEMBER_NAME_CHECKED(UK2Node_SwitchOnDataHandle, PinHandles);

    if (bIsDirty) {
        ReconstructNode();
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
    GetGraph()->NotifyNodeChanged(this);
}

void UK2Node_SwitchOnDataHandle::PostLoad() {
    Super::PostLoad();
    if (UEdGraphPin *FunctionPin = FindPin(FunctionName)) {
        FunctionPin->DefaultObject = FunctionClass->GetDefaultObject();
    }
}

FText UK2Node_SwitchOnDataHandle::GetTooltipText() const {
    return NSLOCTEXT("K2Node", "SwitchTag_ToolTip", "Selects an output that matches the input value");
}

FText UK2Node_SwitchOnDataHandle::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return NSLOCTEXT("K2Node", "Switch_Tag", "Switch on Status Handle");
}

bool UK2Node_SwitchOnDataHandle::ShouldShowNodeProperties() const {
    return true;
}

void UK2Node_SwitchOnDataHandle::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    // actions get registered under specific object-keys; the idea is that 
    // actions might have to be updated (or deleted) if their object-key is  
    // mutated (or removed)... here we use the node's class (so if the node 
    // type disappears, then the action should go with it)
    // to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
    // check to make sure that the registrar is looking for actions of this type
    // (could be regenerating actions for a specific asset, and therefore the 
    // registrar would only accept actions corresponding to that asset)
    if (auto ActionKey = GetClass(); ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        auto NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
        check(NodeSpawner != nullptr);

        ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
    }
}

void UK2Node_SwitchOnDataHandle::AddPinToSwitchNode() {
    FName PinName = GetUniquePinName();
    PinNames.Add(PinName);

    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PinName);
    if (PinHandles.Num() < PinNames.Num()) {
        PinHandles.Add(FStatusHandle());
    }
}

FName UK2Node_SwitchOnDataHandle::GetUniquePinName() {
    FName NewPinName;
    for (auto Rows = FDataManager::GetInstance().GetDataTable<FStatus>().GetTableRowNames(); auto Row : Rows) {
        if (!FindPin(Row)) {
            NewPinName = Row;
            break;
        }
    }

    return NewPinName;
}

FEdGraphPinType UK2Node_SwitchOnDataHandle::GetPinType() const {
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
    PinType.PinSubCategoryObject = FStatusHandle::StaticStruct();
    return PinType;
}

FEdGraphPinType UK2Node_SwitchOnDataHandle::GetInnerCaseType() const {
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
    return PinType;
}

void UK2Node_SwitchOnDataHandle::CreateFunctionPin() {
    // Set properties on the function pin
    UEdGraphPin *FunctionPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, FunctionClass, FunctionName);
    FunctionPin->bDefaultValueIsReadOnly = true;
    FunctionPin->bNotConnectable = true;
    FunctionPin->bHidden = true;

    UFunction *Function = FindUField<UFunction>(FunctionClass, FunctionName);
    const bool bIsStaticFunc = Function->HasAllFunctionFlags(FUNC_Static);
    if (bIsStaticFunc) {
        // Wire up the self to the CDO of the class if it's not us
        if (UBlueprint *BP = GetBlueprint()) {
            UClass *FunctionOwnerClass = Function->GetOuterUClass();
            if (!BP->SkeletonGeneratedClass->IsChildOf(FunctionOwnerClass)) {
                FunctionPin->DefaultObject = FunctionOwnerClass->GetDefaultObject();
            }
        }
    }
}

void UK2Node_SwitchOnDataHandle::CreateSelectionPin() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FStatusHandle::StaticStruct(),
                                 TEXT("Selection"));
}

void UK2Node_SwitchOnDataHandle::CreateCasePins() {
    while (PinHandles.Num() > PinNames.Num()) {
        const FName PinName = GetUniquePinName();
        PinNames.Add(PinName);
    }

    if (PinNames.Num() > PinHandles.Num()) {
        PinNames.SetNum(PinHandles.Num());
    }

    for (int32 Index = 0; Index < PinHandles.Num(); ++Index) {
        PinNames[Index] = PinHandles[Index].RowID;
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PinNames[Index]);
    }
}

void UK2Node_SwitchOnDataHandle::RemovePin(UEdGraphPin *TargetPin) {
    checkSlow(TargetPin != nullptr);

    FName PinName = TargetPin->PinName;
    // Clean-up pin name array
    if (int32 Index = PinNames.IndexOfByKey(PinName); Index >= 0) {
        if (Index < PinHandles.Num()) {
            PinHandles.RemoveAt(Index);
        }
        PinNames.RemoveAt(Index);
    }
}