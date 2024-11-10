// "Unreal Pokémon" created by Retro & Chill.

#include "Data/K2Node_SwitchOnDataHandle.h"
#include "Battle/Status.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DataManager.h"
#include "Ranges/Algorithm/ToArray.h"

UK2Node_SwitchOnDataHandle::UK2Node_SwitchOnDataHandle(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {
    FunctionName = TEXT("NotEqual_HandleHandle");
    FunctionClass = UDataStructHandleUtilities::StaticClass();
    OrphanedPinSaveMode = ESaveOrphanPinMode::SaveNone;
}

void UK2Node_SwitchOnDataHandle::Initialize(UScriptStruct *Struct) {
    StructType = Struct;
}

UScriptStruct *UK2Node_SwitchOnDataHandle::GetStructType() const {
    return StructType;
}

void UK2Node_SwitchOnDataHandle::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (auto PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
        PropertyName == GET_MEMBER_NAME_CHECKED(UK2Node_SwitchOnDataHandle, PinHandles)) {
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
    auto StructName =
        StructType != nullptr ? StructType->GetDisplayNameText() : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "Switch_Tag", "Switch on {Handle}"), TEXT("Handle"), StructName);
}

bool UK2Node_SwitchOnDataHandle::ShouldShowNodeProperties() const {
    return true;
}

void UK2Node_SwitchOnDataHandle::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode, UScriptStruct *Subclass) {
        auto TypedNode = CastChecked<UK2Node_SwitchOnDataHandle>(Node);
        TypedNode->Initialize(Subclass);
    };

    if (auto ActionKey = GetClass(); ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        for (TObjectIterator<UScriptStruct> It; It; ++It) {
            if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(*It, true) ||
                !Pokemon::Data::IsValidDataTableStruct(*It)) {
                continue;
            }

            UBlueprintNodeSpawner *Spawner = UBlueprintNodeSpawner::Create(ActionKey);
            check(Spawner)

            using FCustomizeCallback = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
            Spawner->CustomizeNodeDelegate = FCustomizeCallback::CreateLambda(CustomizeCallback, *It);
            ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
        }
        auto StructTypes = FDataManager::GetInstance().GetStructTypes();

        auto NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
        check(NodeSpawner != nullptr)
        ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
    }
}

void UK2Node_SwitchOnDataHandle::AddPinToSwitchNode() {
    FName PinName = GetUniquePinName();
    PinNames.Add(PinName);

    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, PinName);
    if (PinHandles.Num() < PinNames.Num()) {
        PinHandles.Add(FDataStructHandle{PinNames.Last()});
    }
}

FName UK2Node_SwitchOnDataHandle::GetUniquePinName() {
    Pokemon::Data::FStructWrapper DataHandle(StructType);
    auto Options = DataHandle.GetStructOptions();
    auto Rows = Options | UE::Ranges::Map([](const TSharedPtr<FString> &String) { return FName(**String); }) |
                UE::Ranges::ToArray;
    FName NewPinName;
    for (auto Row : Rows) {
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
    PinType.PinSubCategoryObject = StructType;
    return PinType;
}

FEdGraphPinType UK2Node_SwitchOnDataHandle::GetInnerCaseType() const {
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
    return PinType;
}

void UK2Node_SwitchOnDataHandle::CreateFunctionPin() {
    // Set properties on the function pin
    auto FunctionPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, FunctionClass, FunctionName);
    FunctionPin->bDefaultValueIsReadOnly = true;
    FunctionPin->bNotConnectable = true;
    FunctionPin->bHidden = true;

    const auto Function = FindUField<UFunction>(FunctionClass, FunctionName);
    const bool bIsStaticFunc = Function->HasAllFunctionFlags(FUNC_Static);
    if (bIsStaticFunc) {
        // Wire up the self to the CDO of the class if it's not us
        if (auto BP = GetBlueprint()) {
            if (const auto FunctionOwnerClass = Function->GetOuterUClass();
                !BP->SkeletonGeneratedClass->IsChildOf(FunctionOwnerClass)) {
                FunctionPin->DefaultObject = FunctionOwnerClass->GetDefaultObject();
            }
        }
    }
}

void UK2Node_SwitchOnDataHandle::CreateSelectionPin() {
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, StructType, TEXT("Selection"));
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