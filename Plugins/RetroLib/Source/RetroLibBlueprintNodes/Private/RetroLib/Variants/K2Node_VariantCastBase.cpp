// "Unreal Pokémon" created by Retro & Chill.

#include "RetroLib/Variants/K2Node_VariantCastBase.h"
#include "K2Node_IfThenElse.h"
#include "KismetCompiler.h"
#include "RetroLib/Blueprints/BlueprintPins.h"

void UK2Node_VariantCastBase::AllocateDefaultPins() {
    if (!bIsPure) {
        CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_CastFailed);
    }

    CreateInputAndOutputPins();

    if (bIsPure) {
        CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, Retro::PN_CastSucceeded);
    }
}

bool UK2Node_VariantCastBase::IsNodePure() const {
    return bIsPure;
}

FLinearColor UK2Node_VariantCastBase::GetNodeTitleColor() const {
    return FLinearColor(0.0f, 0.55f, 0.62f);
}

FSlateIcon UK2Node_VariantCastBase::GetIconAndTint(FLinearColor &OutColor) const {
    static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Cast_16x");
    return Icon;
}

void UK2Node_VariantCastBase::GetNodeContextMenuActions(UToolMenu *Menu, UGraphNodeContextMenuContext *Context) const {
    Super::GetNodeContextMenuActions(Menu, Context);

    if (Context->bIsDebugging) {
        return;
    }

    FText MenuEntryTitle = NSLOCTEXT("UK2Node_VariantCastBase", "MakePureTitle", "Convert to pure cast");
    FText MenuEntryTooltip = NSLOCTEXT("UK2Node_VariantCastBase", "MakePureTooltip",
                                       "Removes the execution pins to make the node more versatile (NOTE: the cast "
                                       "could still fail, resulting in an invalid output).");

    bool bCanTogglePurity = true;
    auto CanExecutePurityToggle = [](bool const bInCanTogglePurity) { return bInCanTogglePurity; };

    if (IsNodePure()) {
        MenuEntryTitle = NSLOCTEXT("UK2Node_VariantCastBase", "MakeImpureTitle", "Convert to impure cast");
        MenuEntryTooltip = NSLOCTEXT(
            "UK2Node_VariantCastBase", "MakeImpureTooltip",
            "Adds in branching execution pins so that you can separatly handle when the cast fails/succeeds.");

        const UEdGraphSchema_K2 *K2Schema = Cast<UEdGraphSchema_K2>(GetSchema());
        check(K2Schema != nullptr)

        bCanTogglePurity = K2Schema->DoesGraphSupportImpureFunctions(GetGraph());
        if (!bCanTogglePurity) {
            MenuEntryTooltip = NSLOCTEXT("UK2Node_VariantCastBase", "CannotMakeImpureTooltip",
                                         "This graph does not support impure calls (and you should therefore test the "
                                         "cast's result for validity).");
        }
    }

    FToolMenuSection &Section =
        Menu->AddSection("UK2Node_VariantCastBase", NSLOCTEXT("UK2Node_VariantCastBase", "DynamicCastHeader", "Cast"));
    Section.AddMenuEntry("TogglePurity", MenuEntryTitle, MenuEntryTooltip, FSlateIcon(),
                         FUIAction(FExecuteAction::CreateUObject(const_cast<UK2Node_VariantCastBase *>(this),
                                                                 &UK2Node_VariantCastBase::TogglePurity),
                                   FCanExecuteAction::CreateStatic(CanExecutePurityToggle, bCanTogglePurity),
                                   FIsActionChecked()));
}

void UK2Node_VariantCastBase::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) {
    Super::ExpandNode(CompilerContext, SourceGraph);

    // FUNCTION NODE
    auto [CallPerformCast, CallInputPin, CallOutputPin] = GetPerformCastNode(CompilerContext, SourceGraph);
    auto CallCastResultPin = CallPerformCast->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

    auto InputPin = GetInputPin();
    auto ReturnValuePin = GetOutputPin();

    CallInputPin->PinType = InputPin->PinType;
    CallInputPin->PinType.PinSubCategoryObject = InputPin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*InputPin, *CallInputPin);

    CallOutputPin->PinType = ReturnValuePin->PinType;
    CallOutputPin->PinType.PinSubCategoryObject = ReturnValuePin->PinType.PinSubCategoryObject;
    CompilerContext.MovePinLinksToIntermediate(*ReturnValuePin, *CallOutputPin);

    MakeAdditionalPinLinks(*CallPerformCast);

    if (!bIsPure) {
        auto ExecPin = GetExecPin();
        auto ThenPin = GetThenPin();
        auto CastFailedPin = GetCastFailedPin();

        auto BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
        BranchNode->AllocateDefaultPins();
        CallCastResultPin->MakeLinkTo(BranchNode->GetConditionPin());
        CompilerContext.MovePinLinksToIntermediate(*ExecPin, *BranchNode->GetExecPin());
        CompilerContext.MovePinLinksToIntermediate(*ThenPin, *BranchNode->GetThenPin());
        CompilerContext.MovePinLinksToIntermediate(*CastFailedPin, *BranchNode->GetElsePin());
    } else {
        CompilerContext.MovePinLinksToIntermediate(*GetCastSucceededPin(), *CallCastResultPin);
    }

    BreakAllNodeLinks();
}

UEdGraphPin *UK2Node_VariantCastBase::GetCastFailedPin() const {
    return FindPin(UEdGraphSchema_K2::PN_CastFailed);
}

void UK2Node_VariantCastBase::MakeAdditionalPinLinks(UK2Node &CallPerformCast) const {
    // In the base class this is a no-op
}

UEdGraphPin *UK2Node_VariantCastBase::GetCastSucceededPin() const {
    return FindPin(Retro::PN_CastSucceeded);
}

void UK2Node_VariantCastBase::TogglePurity() {
    const bool bIsNodePure = IsNodePure();
    const FText TransactionTitle =
        bIsNodePure ? NSLOCTEXT("UK2Node_VariantCastBase", "TogglePurityToImpure", "Convert to Impure Cast")
                    : NSLOCTEXT("UK2Node_VariantCastBase", "TogglePurityToPure", "Convert to Pure Cast");
    const FScopedTransaction Transaction(TransactionTitle);
    Modify();
    SetPurity(!bIsNodePure);
}

void UK2Node_VariantCastBase::SetPurity(bool bPurity) {
    if (bPurity == bIsPure) {
        return;
    }

    bIsPure = bPurity;
    if (Pins.Num() > 0) {
        ReconstructNode();
    }
}