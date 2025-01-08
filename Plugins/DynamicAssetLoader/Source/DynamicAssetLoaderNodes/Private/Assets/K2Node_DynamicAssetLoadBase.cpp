// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/K2Node_DynamicAssetLoadBase.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "RetroLib/Optionals/OrElseValue.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include <algorithm>

static const FName AssetNamePinName = "AssetName";
static const FName AssetNotFoundPinName = "AssetNotFound";

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
    auto AssetNamePin = CreatePin(
        EGPD_Input, bWildcardMode ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_String, AssetNamePinName);
    SetPinToolTip(*AssetNamePin,
                  NSLOCTEXT("K2Node", "DynamicAssetLoadBase AssetNamePinDescription", "The name of the asset to load"));

    // Result pin
    auto Setting = GetDefault<UAssetLoadingSettings>();
    auto &ClassType = Setting->AssetClasses.FindChecked(AssetKey);
    auto ResultPin = CreateResultsPin(ClassType.AssetClass);

    ResultPin->PinFriendlyName = NSLOCTEXT("K2Node", "DynamicAssetLoadBase Output Asset", "Found Asset");
    SetPinToolTip(*ResultPin,
                  NSLOCTEXT("K2Node", "DynamicAssetLoadBase ResultPinDescription", "The returned asset, if found"));

    Super::AllocateDefaultPins();
}

void UK2Node_DynamicAssetLoadBase::PostReconstructNode() {
    Super::PostReconstructNode();
    RefreshAssetNamePin();
}

bool UK2Node_DynamicAssetLoadBase::IsConnectionDisallowed(const UEdGraphPin *MyPin, const UEdGraphPin *OtherPin,
                                                          FString &OutReason) const {
    static const std::array ValidPinTypes = {UEdGraphSchema_K2::PC_Name, UEdGraphSchema_K2::PC_String,
                                             UEdGraphSchema_K2::PC_Text};

    if (MyPin != GetAssetNamePin() || MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard) {
        return false;
    }

    if (std::ranges::find(ValidPinTypes, OtherPin->PinType.PinCategory) == ValidPinTypes.end()) {
        OutReason = TEXT("Not a valid string type structure!");
        return true;
    }

    return false;
}

void UK2Node_DynamicAssetLoadBase::NotifyPinConnectionListChanged(UEdGraphPin *Pin) {
    Super::NotifyPinConnectionListChanged(Pin);

    if (Pin != GetAssetNamePin()) {
        return;
    }

    RefreshAssetNamePin();
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

        CachedNodeTitle.SetCachedText(FText::FormatNamed(LocFormat, TEXT("AssetClass"), ClassType.DisplayName), this);
    }

    return CachedNodeTitle;
}

FText UK2Node_DynamicAssetLoadBase::GetTooltipText() const {
    return NodeTooltip;
}

void UK2Node_DynamicAssetLoadBase::GetNodeContextMenuActions(UToolMenu *Menu,
                                                             UGraphNodeContextMenuContext *Context) const {
    Super::GetNodeContextMenuActions(Menu, Context);

    if (Context->bIsDebugging) {
        return;
    }

    FText MenuEntryTitle =
        NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "MakeStringLiteralTitle", "Convert to String-literal parameter");
    FText MenuEntryTooltip =
        NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "MakeStringLiteralTooltip",
                  "Converts the asset name pin to a string literal parameter, allowing a hard-coded value to be used");

    if (!bWildcardMode) {
        MenuEntryTitle =
            NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "MakeWildcardTitle", "Convert to wildcard parameter");
        MenuEntryTooltip = NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "MakeWildcardTooltip",
                                     "Converts the asset name pin to a wildcard parameter, allowing a Name, String or "
                                     "Text pin to be connected without conversion");
    }

    FToolMenuSection &Section = Menu->AddSection(
        "UK2Node_DynamicAssetLoadBase", NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "SearchHeader", "Parameters"));

    Section.AddMenuEntry("ToggleWildcard", MenuEntryTitle, MenuEntryTooltip, FSlateIcon(),
                         FUIAction(FExecuteAction::CreateUObject(const_cast<UK2Node_DynamicAssetLoadBase *>(this),
                                                                 &UK2Node_DynamicAssetLoadBase::ToggleWildcard)));
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
    CallCreateAssetNamePin->PinType = AssetNamePin->PinType;
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
    return NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "MenuCategory", "Assets");
}

UEdGraphPin *UK2Node_DynamicAssetLoadBase::GetAssetFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin *UK2Node_DynamicAssetLoadBase::GetAssetNamePin() const {
    UEdGraphPin *Pin = FindPinChecked(AssetNamePinName);
    check(Pin->Direction == EGPD_Input)
    return Pin;
}

UEdGraphPin *UK2Node_DynamicAssetLoadBase::GetAssetNotFoundPin() const {
    UEdGraphPin *Pin = FindPinChecked(AssetNotFoundPinName);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UEdGraphPin *UK2Node_DynamicAssetLoadBase::GetResultPin() const {
    UEdGraphPin *Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    check(Pin->Direction == EGPD_Output)
    return Pin;
}

UClass *UK2Node_DynamicAssetLoadBase::GetAssetClassType() const {
    auto Setting = GetDefault<UAssetLoadingSettings>();
    // clang-format off
    return &(Retro::Optionals::OfNullable(Setting->AssetClasses.Find(AssetKey)) |
             Retro::Optionals::Transform(&FAssetLoadingEntry::AssetClass) |
             Retro::Optionals::AndThen(&FAssetClassType::TryGet<UClass>) |
             Retro::Optionals::OrElseValue(std::ref(*UObject::StaticClass())));
    // clang-format on
}

void UK2Node_DynamicAssetLoadBase::SetWildcardMode(bool bNewWildcardMode) {
    if (bNewWildcardMode == bWildcardMode) {
        return;
    }

    bWildcardMode = bNewWildcardMode;
    if (bWildcardMode) {
        RefreshAssetNamePin();
        return;
    }

    auto AssetNamePin = GetAssetNamePin();
    // clang-format off
    auto LinkedPins = AssetNamePin->LinkedTo |
                   Retro::Ranges::Views::Filter([](const UEdGraphPin *Pin) { return Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_String; }) |
                       Retro::Ranges::To<TArray>();
    LinkedPins | Retro::Ranges::ForEach(Retro::BindMethod<&UEdGraphPin::BreakLinkTo>(AssetNamePin, true));
    // clang-format on
    RefreshAssetNamePin();
}

void UK2Node_DynamicAssetLoadBase::SetPinToolTip(UEdGraphPin &MutatablePin, const FText &PinDescription) const {
    MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

    if (auto K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema()); K2Schema != nullptr) {
        MutatablePin.PinToolTip += TEXT(" ");
        MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
    }

    MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UK2Node_DynamicAssetLoadBase::ToggleWildcard() {
    const FText TransactionTitle =
        bWildcardMode ? NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "ToggleToString", "Convert to String param")
                      : NSLOCTEXT("UK2Node_DynamicAssetLoadBase", "ToggleToWildcard", "Convert to Wildcard Param");
    const FScopedTransaction Transaction(TransactionTitle);
    Modify();
    SetWildcardMode(!bWildcardMode);
}

void UK2Node_DynamicAssetLoadBase::RefreshAssetNamePin() const {
    if (auto NamePin = GetAssetNamePin(); NamePin->LinkedTo.Num() > 0) {
        check(NamePin->LinkedTo.Num() == 1)
        auto Pin = NamePin->LinkedTo[0];
        NamePin->PinType = Pin->PinType;
    } else {
        NamePin->PinType.PinCategory = bWildcardMode ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_String;
    }
}