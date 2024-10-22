// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/K2Node_DynamicAssetLoadBase.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "Ranges/Optional/FlatMap.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OrElse.h"


static const FName AssetNamePinName = "AssetName";
static const FName AssetNotFoundPinName = "AssetNotFound";

UK2Node_DynamicAssetLoadBase::UK2Node_DynamicAssetLoadBase() {
}

void UK2Node_DynamicAssetLoadBase::Initialize(FName InAssetKey) {
    AssetKey = InAssetKey;
}

void UK2Node_DynamicAssetLoadBase::AllocateDefaultPins() {
    // Add execution pins
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
    auto AssetFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
    AssetFoundPin->PinFriendlyName = NSLOCTEXT("K2Node", "DynamicAssetLoadBase Asset Found Exec pin", "Asset Found");
    CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, AssetNotFoundPinName);

    // AssetNamePin
    auto AssetNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, AssetNamePinName);
    SetPinToolTip(*AssetNamePin, NSLOCTEXT("K2Node", "DynamicAssetLoadBase AssetNamePinDescription", "The name of the asset to load"));

    // Result pin
    auto Setting = GetDefault<UAssetLoadingSettings>();
    auto &ClassType = Setting->AssetClasses.FindChecked(AssetKey);
    auto ResultPin = CreateResultsPin(ClassType.AssetClass);

    ResultPin->PinFriendlyName = NSLOCTEXT("K2Node", "DynamicAssetLoadBase Output Asset", "Found Asset");
    SetPinToolTip(*ResultPin,
                  NSLOCTEXT("K2Node", "DynamicAssetLoadBase ResultPinDescription", "The returned asset, if found"));

    Super::AllocateDefaultPins();
}

FText UK2Node_DynamicAssetLoadBase::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    auto LocFormat = GetNodeTitleFormat();
    if (auto AssetClass = GetDefault<UAssetLoadingSettings>()->AssetClasses.Find(AssetKey); AssetClass == nullptr) {
        return FText::FormatNamed(LocFormat, "Asset",
            NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "GetNodeTitle Asset", "Asset"));
    }

    if (CachedNodeTitle.IsOutOfDate(this)) {
        auto Setting = GetDefault<UAssetLoadingSettings>();
        auto &ClassType = Setting->AssetClasses.FindChecked(AssetKey);

        CachedNodeTitle.SetCachedText(FText::FormatNamed(LocFormat,
            TEXT("AssetClass"), ClassType.DisplayName), this);
    }

    return CachedNodeTitle;
}

FText UK2Node_DynamicAssetLoadBase::GetTooltipText() const {
    return NodeTooltip;
}

void UK2Node_DynamicAssetLoadBase::ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    const FName FunctionName = GetLoadFunctionName();
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

    CallCreateFoundAssetPin->PinType = ReturnValuePin->PinType;
    CallCreateFoundAssetPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*FoundPin, *CallCreateFoundPin);
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallCreateFoundAssetPin);

    CompilerContext.MovePinLinksToIntermediate(*NotFoundPin, *CallCreateNotFoundPin);

    BreakAllNodeLinks();
}

FSlateIcon UK2Node_DynamicAssetLoadBase::GetIconAndTint(FLinearColor &OutColor) const {
    OutColor = GetNodeTitleColor();
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
    return Icon;
}

void UK2Node_DynamicAssetLoadBase::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
    auto CustomizeCallback = [](UEdGraphNode *Node, [[maybe_unused]] bool bIsTemplateNode, FName InAssetKey) {
        auto TypedNode = CastChecked<UK2Node_DynamicAssetLoadBase>(Node);
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

FText UK2Node_DynamicAssetLoadBase::GetMenuCategory() const {
    return Super::GetMenuCategory();
}

UEdGraphPin * UK2Node_DynamicAssetLoadBase::GetAssetFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin * UK2Node_DynamicAssetLoadBase::GetAssetNamePin() const {
    UEdGraphPin *Pin = FindPinChecked(AssetNamePinName);
    check(Pin->Direction == EGPD_Input)
    return Pin;
}

UEdGraphPin * UK2Node_DynamicAssetLoadBase::GetAssetNotFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(AssetNotFoundPinName);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin * UK2Node_DynamicAssetLoadBase::GetResultPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UClass * UK2Node_DynamicAssetLoadBase::GetAssetClassType() const {
    auto Setting = GetDefault<UAssetLoadingSettings>();
    // clang-format off
    return &(UE::Optionals::OfNullable(Setting->AssetClasses.Find(AssetKey)) |
        UE::Optionals::Map(&FAssetLoadingEntry::AssetClass) |
        UE::Optionals::FlatMap(&FAssetClassType::TryGet<UClass>) |
        UE::Optionals::OrElse(*UObject::StaticClass()));
    // clang-format on
}

void UK2Node_DynamicAssetLoadBase::SetPinToolTip(UEdGraphPin &MutatablePin, const FText &PinDescription) const {
    MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

    if (auto K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema()); K2Schema != nullptr) {
        MutatablePin.PinToolTip += TEXT(" ");
        MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
    }

    MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}