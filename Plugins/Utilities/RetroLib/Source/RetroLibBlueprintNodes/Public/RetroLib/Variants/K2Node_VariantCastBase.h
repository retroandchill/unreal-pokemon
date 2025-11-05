// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_VariantOperationBase.h"

#include "K2Node_VariantCastBase.generated.h"

/**
 * @class UK2Node_VariantCastBase
 * @brief A base node for casting between variant types and objects in the Unreal Engine 5 Kismet system.
 *
 * The UK2Node_VariantCastBase class serves as a foundational node class in UE5's Kismet visual scripting language.
 * It enables the casting of variant data types, providing a mechanism to handle different data conversions
 * dynamically at runtime within Blueprint scripts.
 *
 * @details
 * This class provides the essential infrastructure for creating custom nodes that can perform type conversions
 * between various variant data types. It is intended to be derived from in order to implement specific casting
 * functionality required by the user. Since this is a base class, it does not directly implement the casting
 * logic but rather sets up the necessary context and framework for derived classes to define their conversion rules.
 *
 * Primary use cases include:
 * - Implementing custom data type conversions within Blueprint nodes.
 * - Extending Blueprint functionality with new custom cast nodes.
 * - Managing dynamic type conversions in a visual script environment.
 *
 * Key responsibilities:
 * - Define the basic interface and structure for a variant cast node.
 * - Facilitate the integration of custom cast logic in derived classes.
 */
UCLASS(Abstract)
class RETROLIBBLUEPRINTNODES_API UK2Node_VariantCastBase : public UK2Node_VariantOperationBase {
    GENERATED_BODY()

  protected:
    struct FCastFunctionInfo {
        UK2Node *CastNode = nullptr;
        UEdGraphPin *InputPin = nullptr;
        UEdGraphPin *OutputPin = nullptr;
    };

  public:
    void AllocateDefaultPins() override;
    bool IsNodePure() const override;
    FLinearColor GetNodeTitleColor() const override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    void GetNodeContextMenuActions(UToolMenu *Menu, UGraphNodeContextMenuContext *Context) const override;
    void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;

  protected:
    virtual void CreateInputAndOutputPins() ABSTRACT_METHOD
    UEdGraphPin *GetCastFailedPin() const;

    virtual void MakeAdditionalPinLinks(UK2Node &CallPerformCast) const;
    virtual UEdGraphPin *GetInputPin() const ABSTRACT_METHOD
    virtual UEdGraphPin *GetOutputPin() const ABSTRACT_METHOD
    UEdGraphPin *GetCastSucceededPin() const;

    virtual FCastFunctionInfo GetPerformCastNode(FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph)
        ABSTRACT_METHOD

  private:
    void TogglePurity();
    void SetPurity(bool bPurity);

    UPROPERTY()
    bool bIsPure = false;
};
