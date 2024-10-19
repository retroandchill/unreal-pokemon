// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/K2Node_MakeSoftVariantFromSoftObject.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Ranges/Variants/VariantObjectUtilities.h"

void UK2Node_MakeSoftVariantFromSoftObject::Initialize(UClass* Object, UScriptStruct *SoftReference) {
    ObjectType = Object;
    SoftReferenceType = SoftReference;
}

void UK2Node_MakeSoftVariantFromSoftObject::AllocateDefaultPins() {
    auto InputPinSubclass = ObjectType != nullptr ? ObjectType.Get() : UObject::StaticClass();
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_SoftObject, InputPinSubclass, ObjectType->GetFName());
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct,
              (SoftReferenceType != nullptr ? SoftReferenceType.Get() : FTableRowBase::StaticStruct()),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_MakeSoftVariantFromSoftObject::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto ClassName = ObjectType != nullptr
                          ? ObjectType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName = SoftReferenceType != nullptr
                          ? SoftReferenceType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromSoftObject_GetNodeTitle", "Make {Output} ({Input})"),
                              TEXT("Input"), ClassName, TEXT("Output"), StructName);
}

FText UK2Node_MakeSoftVariantFromSoftObject::GetTooltipText() const {
    auto ClassName = ObjectType != nullptr
                          ? ObjectType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    auto StructName = SoftReferenceType != nullptr
                          ? SoftReferenceType->GetDisplayNameText()
                          : FText::FromStringView(TEXT("<<INVALID>>"));
    return FText::FormatNamed(NSLOCTEXT("K2Node", "MakeSoftVariantFromSoftObject_GetNodeTitle", "Make a new {Output} from the specified {Input}"),
                              TEXT("Input"), ClassName, TEXT("Output"), StructName);
}

void UK2Node_MakeSoftVariantFromSoftObject::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UVariantObjectUtilities, MakeSoftVariantFromSoftObject);
    auto CallCreateVariant = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallCreateVariant->FunctionReference.SetExternalMember(FunctionName, UVariantObjectUtilities::StaticClass());
    CallCreateVariant->AllocateDefaultPins();

    static const FName Object_ParamName(TEXT("Object"));
    static const FName Variant_ParamName(TEXT("SoftVariant"));

    auto InputPin = FindPinChecked(ObjectType->GetFName());
    auto ReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto CallCreateObjectPin = CallCreateVariant->FindPinChecked(Object_ParamName);
    auto CallCreateVariantPin = CallCreateVariant->FindPinChecked(Variant_ParamName);

    CallCreateObjectPin->PinType = InputPin->PinType;
    CallCreateObjectPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallCreateObjectPin);

    CallCreateVariantPin->PinType = ReturnValuePin->PinType;
    CallCreateVariantPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateVariantPin);

    BreakAllNodeLinks();
}

void UK2Node_MakeSoftVariantFromSoftObject::AddMenuOptionsForStruct(FBlueprintActionDatabaseRegistrar &ActionRegistrar,
    UE::Ranges::IVariantRegistration &Registration) const {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    auto CustomizeCallback = [](UEdGraphNode *Node, bool,
                                UClass *Input, UScriptStruct *Output) {
        auto TypedNode = CastChecked<UK2Node_MakeSoftVariantFromSoftObject>(Node);
        TypedNode->Initialize(Input, Output);
    };

    auto ActionKey = GetClass();
    auto Struct = Registration.GetSoftStructType();
    TSet<UClass*> AddedClasses;
    TArray<UClass*> Interfaces;
    for (auto Classes = Registration.GetValidClasses(); auto Class : Classes) {
        if (Class->HasAnyClassFlags(CLASS_Interface)) {
            Interfaces.Add(Class);
            continue;
        }
        
        auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
        check(Spawner != nullptr);
        Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateStatic(CustomizeCallback, Class, Struct);
        ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
        AddedClasses.Add(Class);
    }

    for (TObjectIterator<UClass> It; It; ++It) {
        if (AddedClasses.Contains(*It)) {
            continue;
        }

        for (auto Interface : Interfaces) {
            if (!It->ImplementsInterface(Interface) || It->GetSuperClass()->ImplementsInterface(Interface)) {
                continue;
            }

            auto Spawner = UBlueprintNodeSpawner::Create(ActionKey);
            check(Spawner != nullptr);
            Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateStatic(CustomizeCallback, *It, Struct);
            ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
            AddedClasses.Add(*It);
        }
    }
}