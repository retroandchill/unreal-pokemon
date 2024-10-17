// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/K2Node_LookupAssetByName.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/OrElse.h"

class UK2Node_CallFunction;
static const FName Lookup_AssetNamePinName = "AssetName";
static const FName Lookup_AssetNotFoundPinName = "AssetNotFound";

UK2Node_LookupAssetByName::UK2Node_LookupAssetByName() {
    NodeTooltip = NSLOCTEXT("UK2Node_LookupAssetByName", "NodeTooltip", "Attempts to find the path of an asset by its name");
}

void UK2Node_LookupAssetByName::Initialize(FName InAssetKey) {
    AssetKey = InAssetKey;
}

void UK2Node_LookupAssetByName::AllocateDefaultPins() {
    // Add execution pins
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    auto AssetFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    AssetFoundPin->PinFriendlyName = NSLOCTEXT("K2Node", "LoadAssetByName Asset Found Exec pin", "Asset Found");
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, Lookup_AssetNotFoundPinName);

    // AssetNamePin
    auto AssetNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, Lookup_AssetNamePinName);
    SetPinToolTip(*AssetNamePin, NSLOCTEXT("K2Node", "AssetNamePinDescription", "The name of the asset to load"));

    // Result pin
    auto Setting = GetDefault<UAssetLoadingSettings>();
    auto &ClassType = Setting->AssetClasses.FindChecked(AssetKey);
    UEdGraphPin *ResultPin;
    if (ClassType.AssetClass.IsType<UClass>()) {
        ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_SoftObject, &ClassType.AssetClass.Get<UClass>(),
                              UEdGraphSchema_K2::PN_ReturnValue);
    } else {
        check(ClassType.AssetClass.IsType<UScriptStruct>())
        // clang-format off
        auto SoftStruct = UE::Ranges::FVariantObjectStructRegistry::Get().GetVariantStructData(ClassType.AssetClass.Get<UScriptStruct>()) |
            UE::Optionals::Map(&UE::Ranges::IVariantRegistration::GetSoftStructType) |
            UE::Optionals::GetPtrOrNull;
        // clang-format on
        ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, SoftStruct, UEdGraphSchema_K2::PN_ReturnValue);
    }

    ResultPin->PinFriendlyName = NSLOCTEXT("K2Node", "LoadAssetByName Output Asset", "Found Asset");
    SetPinToolTip(*ResultPin,
                  NSLOCTEXT("K2Node", "LoadAssetByName ResultPinDescription", "The returned asset, if found"));

    Super::AllocateDefaultPins();
}

FText UK2Node_LookupAssetByName::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    if (auto AssetClass = GetDefault<UAssetLoadingSettings>()->AssetClasses.Find(AssetKey); AssetClass == nullptr) {
        return NSLOCTEXT("K2Node", "LookupAssetByName_Title_None", "Lookup Asset by Name");
    }

    if (CachedNodeTitle.IsOutOfDate(this)) {
        FFormatNamedArguments Args;
        Args.Add(TEXT("AssetClass"), FText::FromString(AssetKey.ToString()));

        FText LocFormat = NSLOCTEXT("K2Node", "LookupAssetByNameTitle", "Lookup {AssetClass} by Name");
        CachedNodeTitle.SetCachedText(FText::Format(LocFormat, Args), this);
    }

    return CachedNodeTitle;
}

FText UK2Node_LookupAssetByName::GetTooltipText() const {
    return NodeTooltip;
}

void UK2Node_LookupAssetByName::ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UAssetLoader, LookupDynamicAsset);
    auto CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallGetNode->FunctionReference.SetExternalMember(FunctionName, UAssetLoader::StaticClass());
    CallGetNode->AllocateDefaultPins();

    static const FName IdentifierKey_ParamName(TEXT("Identifier"));
    static const FName AssetName_ParamName(TEXT("AssetName"));
    static const FName FoundAsset_ParamName(TEXT("FoundAsset"));
    static const FName Found_ParamName(TEXT("Found"));
    static const FName NotFound_ParamName(TEXT("NotFound"));

    auto ExecPin = GetExecPin();
    auto AssetNamePin = GetAssetNamePin();
    auto ReturnValuePin = GetResultPin();
    auto FoundPin = GetAssetFoundPin();
    auto NotFoundPin = GetAssetNotFoundPin();

    auto CallCreateExecutePin = CallGetNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
    auto CallCreateIdentifierPin = CallGetNode->FindPinChecked(IdentifierKey_ParamName);
    auto CallCreateAssetNamePin = CallGetNode->FindPinChecked(AssetName_ParamName);
    auto CallCreateFoundAssetPin = CallGetNode->FindPinChecked(FoundAsset_ParamName);
    auto CallCreateFoundPin = CallGetNode->FindPinChecked(Found_ParamName);
    auto CallCreateNotFoundPin = CallGetNode->FindPinChecked(NotFound_ParamName);

    CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallCreateExecutePin);
    CallCreateIdentifierPin->DefaultValue = AssetKey.ToString();
    CompilerContext.MovePinLinksToIntermediate(*AssetNamePin, *CallCreateAssetNamePin);

    if (auto &ClassType = GetDefault<UAssetLoadingSettings>()->AssetClasses.FindChecked(AssetKey);
        ClassType.AssetClass.IsType<UClass>()) {
        CallCreateFoundAssetPin->PinType = ReturnValuePin->PinType;
        CallCreateFoundAssetPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
        CompilerContext.MovePinLinksToIntermediate(*FoundPin, *CallCreateFoundPin);
        CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateFoundAssetPin);
    } else {
        check(ClassType.AssetClass.IsType<UScriptStruct>())
        const auto &Struct = ClassType.AssetClass.Get<UScriptStruct>();
        auto HelperClassName = FString::Format(TEXT("{0}Helpers"), {Struct.GetName()});
        auto HelperClass = FindObject<UClass>(Struct.GetPackage(), *HelperClassName);
        check(HelperClass != nullptr)
        
        auto CastFunctionName = FString::Format(TEXT("MakeSoft{0}FromSoftObjectPtr"), {Struct.GetName()});
        auto CallCastNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
        CallCastNode->FunctionReference.SetExternalMember(*CastFunctionName, HelperClass);
        CallCastNode->AllocateDefaultPins();

        auto CastObjectName = FString::Format(TEXT("AsSoft{0}"), {Struct.GetName()});
        auto CallCastExecutePin = CallCastNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
        auto CallCastObjectPin = CallCastNode->FindPinChecked(FName("Object"));
        auto CallCastFoundAssetPin = CallCastNode->FindPinChecked(*CastObjectName);
        auto CallCastFoundPin = CallCastNode->FindPinChecked(FName("CastSucceeded"));
        auto CallCastNotFoundPin = CallCastNode->FindPinChecked(FName("CastFailed"));

        CallCreateFoundPin->MakeLinkTo(CallCastExecutePin);
        CallCreateFoundAssetPin->MakeLinkTo(CallCastObjectPin);

        CompilerContext.MovePinLinksToIntermediate(*FoundPin, *CallCastFoundPin);
        CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCastFoundAssetPin);
        CompilerContext.CopyPinLinksToIntermediate(*NotFoundPin, *CallCastNotFoundPin);
    }

    CompilerContext.MovePinLinksToIntermediate(*NotFoundPin, *CallCreateNotFoundPin);

    BreakAllNodeLinks();
}

FSlateIcon UK2Node_LookupAssetByName::GetIconAndTint(FLinearColor &OutColor) const {
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_LookupAssetByName::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode, FName InAssetKey) {
        auto TypedNode = CastChecked<UK2Node_LookupAssetByName>(Node);
        TypedNode->Initialize(InAssetKey);
    };

    if (auto ActionKey = GetClass(); ActionRegistrar.IsOpenForRegistration(ActionKey)) {

        for (auto Settings = GetDefault<UAssetLoadingSettings>(); auto &[Key, AssetClass] : Settings->AssetClasses) {
            if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(
                    &AssetClass.AssetClass.TryGet<UClass>().Get(*UObject::StaticClass()), true)) {
                continue;
                    }

            UBlueprintNodeSpawner *Spawner = UBlueprintNodeSpawner::Create(ActionKey);
            check(Spawner)

            Spawner->CustomizeNodeDelegate =
                UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateLambda(CustomizeCallback, Key);
            ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
        }
    }
}

FText UK2Node_LookupAssetByName::GetMenuCategory() const {
    return NSLOCTEXT("UK2Node_LookupAssetByName", "MenuCategory", "Assets");
}

UEdGraphPin * UK2Node_LookupAssetByName::GetAssetFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin * UK2Node_LookupAssetByName::GetAssetNamePin() const {
    UEdGraphPin *Pin = FindPinChecked(Lookup_AssetNamePinName);
    check(Pin->Direction == EGPD_Input)
    return Pin;
}

UEdGraphPin * UK2Node_LookupAssetByName::GetAssetNotFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(Lookup_AssetNotFoundPinName);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin * UK2Node_LookupAssetByName::GetResultPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UClass * UK2Node_LookupAssetByName::GetAssetClassType() const {
    auto Setting = GetDefault<UAssetLoadingSettings>();
    // clang-format off
    return &(UE::Optionals::OfNullable(Setting->AssetClasses.Find(AssetKey)) |
        UE::Optionals::Map(&FAssetLoadingEntry::AssetClass) |
        UE::Optionals::FlatMap(&FAssetClassType::TryGet<UClass>) |
        UE::Optionals::OrElse(*UObject::StaticClass()));
    // clang-format on
}

void UK2Node_LookupAssetByName::SetPinToolTip(UEdGraphPin &MutatablePin, const FText &PinDescription) const {
    MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

    if (auto K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema()); K2Schema != nullptr) {
        MutatablePin.PinToolTip += TEXT(" ");
        MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
    }

    MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}