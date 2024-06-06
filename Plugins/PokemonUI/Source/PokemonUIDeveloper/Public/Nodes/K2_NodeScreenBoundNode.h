// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"

#include "K2_NodeScreenBoundNode.generated.h"

class UScreen;
/**
 * Abstract declaration of a node that creates a separate version for each screen.
 */
UCLASS(Abstract)
class POKEMONUIDEVELOPER_API UK2_NodeScreenBoundNode : public UK2Node_BaseAsyncTask {
    GENERATED_BODY()

  public:
    /**
     * Create the default object
     * @param Initializer The UE provided initializer
     */
    explicit UK2_NodeScreenBoundNode(const FObjectInitializer &Initializer);

    /**
     * Set up the node assigning the struct that this should be retrieving
     * @param NodeClass The screen type for this node
     * @param NodeCounter The internal counter for how many nodes there are
     */
    void Initialize(TSubclassOf<UScreen> NodeClass, TSharedRef<uint32> NodeCounter);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

  protected:
    /**
     * Add the supplied menu actions for each Display Text Screen subclass
     * @param ActionRegistrar Used to register nodes
     * @param FactoryFunc The function used to actually produce the nodes
     */
    void SupplyMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar, UFunction *FactoryFunc) const;

    /**
     * Iterate over each valid screen and perform the provided action
     * @param Action The invokable action to perform
     */
    virtual void ForEachValidScreen(const TFunctionRef<void(UClass *)> &Action) const;

  private:
    /**
     * The class referenced by this node
     */
    UPROPERTY()
    TSubclassOf<UScreen> ScreenType;

    /**
     * How many total screens are there
     */
    TSharedRef<uint32> TotalScreens = MakeShared<uint32>(0);
};
