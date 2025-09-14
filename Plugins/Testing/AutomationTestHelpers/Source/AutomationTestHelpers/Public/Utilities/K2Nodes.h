#pragma once

#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

struct FBlueprintDeleter
{
    void operator()(UBlueprint *TestBP) const
    {
        if (TestBP == nullptr)
            return;

        TestBP->MarkAsGarbage();
        TestBP->Rename(nullptr, nullptr, REN_DontCreateRedirectors);
    }
};

struct FGraphDeleter
{
    void operator()(UEdGraph *TestGraph) const
    {
        if (TestGraph == nullptr)
            return;

        TestGraph->MarkAsGarbage();
    }
};

#define DeclareTestableBP(BPName, GraphName)                                                                           \
    TUniquePtr<UBlueprint, FBlueprintDeleter> BPName;                                                                  \
    TUniquePtr<UEdGraph, FGraphDeleter> GraphName

#define DefineTestableBP(BPName, GraphName)                                                                            \
    BPName = TUniquePtr<UBlueprint, FBlueprintDeleter>(FKismetEditorUtilities::CreateBlueprint(                        \
        AActor::StaticClass(), GetTransientPackage(), TEXT(#BPName), BPTYPE_Normal, UBlueprint::StaticClass(),         \
        UBlueprintGeneratedClass::StaticClass(), NAME_None));                                                          \
    GraphName =                                                                                                        \
        BPName ? TUniquePtr<UEdGraph, FGraphDeleter>(FBlueprintEditorUtils::FindEventGraph(BPName.Get())) : nullptr

#define CleanUpTestableBP(BPName, GraphName)                                                                           \
    BPName.Reset();                                                                                                    \
    GraphName.Reset()

#define MakeTestableBP(BPName, GraphName)                                                                              \
    auto BPName = TUniquePtr<UBlueprint, FBlueprintDeleter>(FKismetEditorUtilities::CreateBlueprint(                   \
        AActor::StaticClass(), GetTransientPackage(), TEXT(#BPName), BPTYPE_Normal, UBlueprint::StaticClass(),         \
        UBlueprintGeneratedClass::StaticClass(), NAME_None));                                                          \
    auto GraphName =                                                                                                   \
        BPName ? TUniquePtr<UEdGraph, FGraphDeleter>(FBlueprintEditorUtils::FindEventGraph(BPName.Get())) : nullptr

#define MakeTestableNode(NodeName, OwningGraph)                                                                        \
    UK2Node *NodeName = NewObject<UK2Node_CallFunction>(/*outer*/ OwningGraph);                                        \
    OwningGraph->AddNode(NodeName)

#define MakeTestPin(OwningNode, PinArray, InPinName, InPinType, PinDirection)                                          \
    UEdGraphPin *InPinName = UEdGraphPin::CreatePin(OwningNode);                                                       \
    InPinName->PinType.PinCategory = InPinType;                                                                        \
    PinArray.Add(InPinName);                                                                                           \
    InPinName->Direction = PinDirection

#define MakeRealNumberTestPin(OwningNode, PinArray, InPinName, PinDirection)                                           \
    UEdGraphPin *InPinName = UEdGraphPin::CreatePin(OwningNode);                                                       \
    InPinName->PinType.PinCategory = UEdGraphSchema_K2::PC_Real;                                                       \
    InPinName->PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;                                                  \
    PinArray.Add(InPinName);                                                                                           \
    InPinName->Direction = PinDirection

#define MakeRealNumberTestPins(OwningNode, OutArray)                                                                   \
    MakeRealNumberTestPin(OwningNode, OutArray, RealPinA, EGPD_Input);                                                 \
    MakeRealNumberTestPin(OwningNode, OutArray, RealPinB, EGPD_Input);                                                 \
    MakeRealNumberTestPin(OwningNode, OutArray, RealOutputPin, EGPD_Output)

#define MakeTestPins(OwningNode, OutArray)                                                                             \
    MakeRealNumberTestPins(OwningNode, OutArray)                                                                       \
        MakeTestPin(OwningNode, OutArray, Int64PinA, UEdGraphSchema_K2::PC_Int64, EGPD_Output);                        \
    MakeTestPin(OwningNode, OutArray, Int64PinB, UEdGraphSchema_K2::PC_Int64, EGPD_Input);                             \
    MakeTestPin(OwningNode, OutArray, BytePinA, UEdGraphSchema_K2::PC_Byte, EGPD_Output);                              \
    MakeTestPin(OwningNode, OutArray, WildPinA, UEdGraphSchema_K2::PC_Wildcard, EGPD_Output);                          \
    MakeTestPin(OwningNode, OutArray, WildPinB, UEdGraphSchema_K2::PC_Wildcard, EGPD_Input);                           \
    MakeTestPin(OwningNode, OutArray, BytePinB, UEdGraphSchema_K2::PC_Byte, EGPD_Input);                               \
    MakeTestPin(OwningNode, OutArray, BoolPinA, UEdGraphSchema_K2::PC_Boolean, EGPD_Output);                           \
    MakeTestPin(OwningNode, OutArray, BoolPinB, UEdGraphSchema_K2::PC_Boolean, EGPD_Input);                            \
    MakeTestPin(OwningNode, OutArray, BoolOutputPin, UEdGraphSchema_K2::PC_Boolean, EGPD_Output);                      \
    MakeTestPin(OwningNode, OutArray, IntPinA, UEdGraphSchema_K2::PC_Int, EGPD_Output);                                \
    MakeTestPin(OwningNode, OutArray, VecInputPinA, UEdGraphSchema_K2::PC_Struct, EGPD_Input);                         \
    VecInputPinA->PinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();                                       \
    MakeTestPin(OwningNode, OutArray, VecInputPinB, UEdGraphSchema_K2::PC_Struct, EGPD_Input);                         \
    VecInputPinB->PinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();                                       \
    MakeTestPin(OwningNode, OutArray, VecOutputPinA, UEdGraphSchema_K2::PC_Struct, EGPD_Output);                       \
    VecOutputPinA->PinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();                                      \
    MakeTestPin(OwningNode, OutArray, Vec2DOutputPinA, UEdGraphSchema_K2::PC_Struct, EGPD_Output);                     \
    Vec2DOutputPinA->PinType.PinSubCategoryObject = TBaseStructure<FVector2D>::Get();                                  \
    MakeTestPin(OwningNode, OutArray, Vec4OutPin, UEdGraphSchema_K2::PC_Struct, EGPD_Output);                          \
    Vec4OutPin->PinType.PinSubCategoryObject = TBaseStructure<FVector4>::Get();                                        \
    MakeTestPin(OwningNode, OutArray, RotOutPin, UEdGraphSchema_K2::PC_Struct, EGPD_Output);                           \
    RotOutPin->PinType.PinSubCategoryObject = TBaseStructure<FRotator>::Get();                                         \
    MakeTestPin(OwningNode, OutArray, QuatOutPin, UEdGraphSchema_K2::PC_Struct, EGPD_Output);                          \
    QuatOutPin->PinType.PinSubCategoryObject = TBaseStructure<FQuat>::Get()

namespace K2::Nodes
{
}