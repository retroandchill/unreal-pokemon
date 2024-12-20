﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Services/K2Node_GetServiceClass.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "Lookup/InjectionUtilities.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Services/GameServiceSubsystem.h"
#include "Services/ServiceUtilities.h"

void UK2Node_GetServiceClass::Initialize(const TSubclassOf<UService> &Service) {
    ServiceClass = Service;
}

void UK2Node_GetServiceClass::AllocateDefaultPins() {
    // If required add the world context pin
    if (GetBlueprint()->ParentClass->HasMetaDataHierarchical(FBlueprintMetadata::MD_ShowWorldContextPin)) {
        CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), TEXT("WorldContext"));
    }

    // Add blueprint pin
    if (!IsValid(ServiceClass)) {
        CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, USubsystem::StaticClass(), TEXT("ServiceClass"));
    }

    // Result pin
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object,
              IsValid(ServiceClass) ? static_cast<UClass *>(ServiceClass) : UService::StaticClass(),
              UEdGraphSchema_K2::PN_ReturnValue);

    Super::AllocateDefaultPins();
}

FText UK2Node_GetServiceClass::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    // clang-format off
    auto Name = UE::Optionals::OfNullable(ServiceClass) |
                UE::Optionals::Map(&UClass::GetDisplayNameText) |
                UE::Optionals::OrElseGet(&FText::FromStringView, TEXT("???"));
    // clang-format on
    return FText::FormatOrdered(NSLOCTEXT("UK2Node_GetServiceClass", "GetNodeTitle", "Get {0}"), Name);
}

FText UK2Node_GetServiceClass::GetTooltipText() const {
    // clang-format off
    return UE::Optionals::OfNullable(ServiceClass) |
           UE::Optionals::Map(&UClass::GetToolTipText, false) |
           UE::Optionals::OrElseGet(&FText::FromStringView, TEXT("???"));
    // clang-format on
}

FText UK2Node_GetServiceClass::GetCompactNodeTitle() const {
    // clang-format off
    return UE::Optionals::OfNullable(ServiceClass) |
           UE::Optionals::Map(&UClass::GetDisplayNameText) |
           UE::Optionals::OrElseGet(&FText::FromStringView, TEXT("???"));
    // clang-format on
}

bool UK2Node_GetServiceClass::ShouldDrawCompact() const {
    return true;
}

bool UK2Node_GetServiceClass::IsNodePure() const {
    return true;
}

FText UK2Node_GetServiceClass::GetMenuCategory() const {
    return NSLOCTEXT("UK2Node_GetServiceClass", "GetMenuCategory", "DependencyInjection|Services");
}

FSlateIcon UK2Node_GetServiceClass::GetIconAndTint(FLinearColor &OutColor) const {
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_GetServiceClass::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto ActionKey = GetClass();
    if (!ActionRegistrar.IsOpenForRegistration(ActionKey)) {
        return;
    }

    // clang-format off
    UnrealInjector::GetAllServices() |
        UE::Ranges::ForEach(&UK2Node_GetServiceClass::AddMenuActionForType, ActionRegistrar, ActionKey);
    // clang-format on
}

void UK2Node_GetServiceClass::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UGameServiceSubsystem, StaticGetService);
    auto CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallGetNode->FunctionReference.SetExternalMember(FunctionName, UGameServiceSubsystem::StaticClass());
    CallGetNode->AllocateDefaultPins();

    static const FName WorldContext_ParamName(TEXT("WorldContext"));
    static const FName Class_ParamName(TEXT("ServiceClass"));

    if (GetBlueprint()->ParentClass->HasMetaDataHierarchical(FBlueprintMetadata::MD_ShowWorldContextPin)) {
        CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(WorldContext_ParamName),
                                                   *CallGetNode->FindPinChecked(WorldContext_ParamName));
    }

    if (IsValid(ServiceClass)) {
        auto ClassPin = CallGetNode->FindPinChecked(Class_ParamName);
        ClassPin->DefaultObject = ServiceClass;
    } else {
        CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(Class_ParamName),
                                                   *CallGetNode->FindPinChecked(Class_ParamName));
    }

    auto SourceReturnPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    auto TargetReturnPin = CallGetNode->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    TargetReturnPin->PinType = SourceReturnPin->PinType;
    CompilerContext.MovePinLinksToIntermediate(*SourceReturnPin, *TargetReturnPin);

    BreakAllNodeLinks();
}

void UK2Node_GetServiceClass::AddMenuActionForType(const TSubclassOf<UService> &Type,
                                                   FBlueprintActionDatabaseRegistrar &ActionRegistrar,
                                                   UClass *ActionKey) {
    using FCustomizeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate;
    constexpr auto CustomizeCallback = [](UEdGraphNode *Node, bool, const TSubclassOf<UService> &Subclass) {
        auto TypedNode = CastChecked<UK2Node_GetServiceClass>(Node);
        TypedNode->Initialize(Subclass);
    };

    UBlueprintNodeSpawner *Spawner = UBlueprintNodeSpawner::Create(ActionKey);
    check(Spawner)

    Spawner->CustomizeNodeDelegate = FCustomizeDelegate::CreateLambda(CustomizeCallback, Type);
    ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
}