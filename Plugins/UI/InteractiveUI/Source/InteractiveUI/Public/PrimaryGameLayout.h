// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "Engine/AssetManager.h"
#include "GameplayTagContainer.h"
#include "Utilities/CommonUIExtensions.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "PrimaryGameLayout.generated.h"

struct FGameplayTag;
struct FStreamableHandle;
class UCommonActivatableWidget;

/**
 * @enum EAsyncWidgetLayerState
 * @brief Represents the states of an asynchronous widget layer operation.
 *
 * This enumeration defines the different phases of lifecycle state transitions
 * during an asynchronous widget layer operation.
 */
enum class EAsyncWidgetLayerState : uint8
{
    /**
     * The load was cancelled.
     */
    Canceled,

    /**
     * The widget was created, pending initialization of properties.
     */
    Initialize,

    /**
     * THe widget has been added to the screen.
     */
    AfterPush
};

/**
 * @class UPrimaryGameLayout
 * @brief Manages the primary game layout, including widget layer stacks and state transitions.
 *
 * This abstract class is designed to handle the primary layout system of the game, focusing on
 * managing activatable widgets and their associated layer stacks. It provides functionality for
 * asynchronous widget loading, layer registration, widget push operations, and state transitions.
 * It also includes utilities for managing dormant states and input suspension.
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class INTERACTIVEUI_API UPrimaryGameLayout : public UCommonUserWidget
{
    GENERATED_BODY()

    /**
     * @brief Default constructor for the UPrimaryGameLayout class.
     *
     * Creates an instance of UPrimaryGameLayout with default initialization.
     *
     * @return A new instance of the UPrimaryGameLayout class.
     */
  public:
    UPrimaryGameLayout() = default;

    /**
     * @brief Retrieves the instance of UPrimaryGameLayout associated with the given world context object.
     *
     * This method returns the UPrimaryGameLayout instance specific to the provided world context object.
     *
     * @param WorldContextObject The object providing context about the game world, generally within a gameplay
     * scenario.
     * @return A pointer to the UPrimaryGameLayout instance corresponding to the specified world context object.
     */
    static UPrimaryGameLayout *GetInstance(const UObject *WorldContextObject);

    /**
     * @brief Retrieves the instance of UPrimaryGameLayout associated with the given player controller.
     *
     * This method returns the UPrimaryGameLayout instance specific to the provided player controller,
     * allowing interaction with the layout for that player's context.
     *
     * @param PlayerController A pointer to the player controller for which the UPrimaryGameLayout instance is needed.
     * It determines the context of the player.
     * @return A pointer to the UPrimaryGameLayout instance corresponding to the specified player controller,
     * or nullptr if the player controller is invalid.
     */
    static UPrimaryGameLayout *GetInstance(const APlayerController *PlayerController);

    /**
     * @brief Retrieves an instance of UPrimaryGameLayout associated with the specified local player.
     *
     * This function provides access to the UPrimaryGameLayout instance that correlates
     * to the given ULocalPlayer, allowing interaction with the game's primary layout for
     * that player's context.
     *
     * @param LocalPlayer A pointer to the local player for whom the UPrimaryGameLayout instance is required.
     * It determines the context of the player.
     * @return A pointer to the UPrimaryGameLayout instance corresponding to the specified local player,
     * or nullptr if the local player is invalid or not associated with a valid layout.
     */
    static UPrimaryGameLayout *GetInstance(ULocalPlayer *LocalPlayer);

    /**
     * @brief Checks if the game layout is in a dormant state.
     *
     * This method determines whether the game layout is currently dormant, meaning it is in an inactive
     * or paused state where no significant interactions or updates occur.
     *
     * @return True if the game layout is dormant; otherwise, false.
     */
    bool IsDormant() const
    {
        return bIsDormant;
    }

    /**
     * @brief Sets the dormancy state of the Primary Game Layout.
     *
     * This method updates the dormancy state of the Primary Game Layout to the specified value
     * and triggers necessary events or logs to reflect the change.
     *
     * @param bNewIsDormant The new dormancy state to set. Set to true to mark as dormant,
     *                      or false to mark as active.
     */
    void SetIsDormant(bool bNewIsDormant);

    /**
     * @brief Pushes a widget to the specified layer stack asynchronously.
     *
     * This method adds a widget to a specified layer stack asynchronously. It provides
     * an option to suspend input until the operation is complete.
     *
     * @param LayerName The gameplay tag representing the name of the layer to push the widget onto.
     * @param bSuspendInputUntilComplete If true, input is suspended until the widget push operation is complete.
     * @param ActivatableWidgetClass The soft class pointer to the widget class to be activated.
     * @return A shared pointer to the streamable handle representing the asynchronous operation.
     */
    template <std::derived_from<UCommonActivatableWidget> ActivatableWidgetT = UCommonActivatableWidget>
    TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(
        FGameplayTag LayerName, bool bSuspendInputUntilComplete,
        TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass)
    {
        return PushWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, bSuspendInputUntilComplete,
                                                               ActivatableWidgetClass,
                                                               [](EAsyncWidgetLayerState, ActivatableWidgetT *) {});
    }

    /**
     * @brief Pushes a widget to the specified layer stack asynchronously.
     *
     * This method handles the asynchronous loading and initialization of a widget, ensures
     * input suspension during the loading process if specified, and invokes state transition
     * callbacks at various points in the widget's lifecycle.
     *
     * @tparam ActivatableWidgetT The type of the activatable widget to be pushed to the layer stack.
     * @param LayerName The tag representing the name of the layer stack where the widget is to be pushed.
     * @param bSuspendInputUntilComplete A boolean flag indicating whether player input should be suspended
     *                                   until the widget is fully loaded and pushed to the stack.
     * @param ActivatableWidgetClass The class of the activatable widget to be instantiated and pushed.
     * @param StateFunc A callback function that receives state transition notifications during the widget's lifecycle.
     *                  The callback is invoked with an enum representing the widget's current state and the widget
     *                  instance (or null for certain states like cancellation).
     * @return A shared pointer to the asynchronous streamable handle managing the loading operation. This handle can
     *         be used to monitor the status or cancel the operation.
     */
    template <std::derived_from<UCommonActivatableWidget> ActivatableWidgetT = UCommonActivatableWidget>
    TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(
        FGameplayTag LayerName, const bool bSuspendInputUntilComplete,
        TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass,
        std::invocable<EAsyncWidgetLayerState, ActivatableWidgetT *> auto StateFunc)
    {
        static FName PushingWidgetToLayer("PushingWidgetToLayer");
        const FName SuspendInputToken =
            bSuspendInputUntilComplete
                ? UCommonUIExtensions::SuspendInputForPlayer(GetOwningPlayer(), PushingWidgetToLayer)
                : NAME_None;

        auto &StreamableManager = UAssetManager::Get().GetStreamableManager();
        TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(
            ActivatableWidgetClass.ToSoftObjectPath(),
            FStreamableDelegate::CreateWeakLambda(
                this, [this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken] {
                    UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

                    ActivatableWidgetT *Widget = PushWidgetToLayerStack<ActivatableWidgetT>(
                        LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT &WidgetToInit) {
                            StateFunc(EAsyncWidgetLayerState::Initialize, &WidgetToInit);
                        });

                    StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
                }));

        // Setup a cancel delegate so that we can resume input if this handler is canceled.
        StreamingHandle->BindCancelDelegate(
            FStreamableDelegate::CreateWeakLambda(this, [this, StateFunc, SuspendInputToken]() {
                UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
                StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
            }));

        return StreamingHandle;
    }

    /**
     * @brief Pushes a widget onto a specific layer stack.
     *
     * This function adds a widget, specified by its class, to the stack of the specified layer.
     * The widget is initialized and managed within the layer.
     *
     * @tparam ActivatableWidgetT The type of the activatable widget to be pushed to the layer stack.
     * @param LayerName The tag representing the name of the layer where the widget will be added.
     * @param ActivatableWidgetClass The class type of the widget to be activated and pushed onto the layer stack.
     * @return A pointer to the widget instance that was added to the layer stack.
     */
    template <std::derived_from<UCommonActivatableWidget> ActivatableWidgetT = UCommonActivatableWidget>
    ActivatableWidgetT *PushWidgetToLayerStack(FGameplayTag LayerName,
                                               TSubclassOf<ActivatableWidgetT> ActivatableWidgetClass)
    {
        return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass,
                                                          [](ActivatableWidgetT &) {});
    }

    /**
     * @brief Pushes a widget to the specified layer stack and initializes it.
     *
     * This method adds an instance of the specified activatable widget class to the
     * specified layer stack. If the layer does not exist, a nullptr is returned.
     *
     * @tparam ActivatableWidgetT The type of the activatable widget to be pushed to the layer stack.
     * @tparam InitFuncT The function to call in order to initialize the widget
     * @param LayerName The name of the layer stack to which the widget should be added.
     * @param ActivatableWidgetClass The class type of the activatable widget to be instantiated and added.
     * @param InitInstanceFunc A callable object used to initialize the widget instance.
     * @return A pointer to the added widget instance if the layer exists, or nullptr if the layer does not exist.
     */
    template <std::derived_from<UCommonActivatableWidget> ActivatableWidgetT = UCommonActivatableWidget,
              std::invocable<ActivatableWidgetT &> InitFuncT>
    ActivatableWidgetT *PushWidgetToLayerStack(const FGameplayTag LayerName,
                                               TSubclassOf<ActivatableWidgetT> ActivatableWidgetClass,
                                               InitFuncT &&InitInstanceFunc)
    {
        auto *Layer = GetLayerWidget(LayerName);
        return Layer != nullptr
                   ? Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, Forward<InitFuncT>(InitInstanceFunc))
                   : nullptr;
    }

    /**
     * @brief Searches for and removes a specified widget from all layers in the game layout.
     *
     * This method iterates through all the layers in the primary game layout and attempts to
     * remove the provided widget if it exists within any of the layers.
     *
     * @param WidgetToRemove Pointer to the widget that needs to be removed from the layout layers.
     */
    void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget *WidgetToRemove);

    /**
     * @brief Retrieves the widget layer associated with a specified gameplay tag.
     *
     * This function searches for and returns the widget container corresponding to the given layer name.
     *
     * @param LayerName The gameplay tag identifying the requested widget layer.
     * @return A pointer to the widget container corresponding to the specified layer name, or nullptr if not found.
     */
    UCommonActivatableWidgetContainerBase *GetLayerWidget(FGameplayTag LayerName) const;

  protected:
    /**
     * @brief Registers a widget layer to the game layout with a specified tag.
     *
     * This method associates a widget container with a gameplay tag, enabling
     * the management of different UI layers within the game layout. It also sets up
     * transition configurations for the layer.
     *
     * @param LayerTag The gameplay tag that uniquely identifies the layer being registered.
     * @param LayerWidget A pointer to the activatable widget container representing the layer to register.
     */
    UFUNCTION(BlueprintCallable, Category = "Layer")
    void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag,
                       UCommonActivatableWidgetContainerBase *LayerWidget);

    /**
     * @brief Handles changes in the dormant state of the widget layer.
     *
     * This function is triggered whenever the dormant state of the widget layer changes.
     * Implement this function to define custom behavior for when the dormant state transitions.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Layer")
    void OnIsDormantChanged();

    /**
     * @brief Handles the state changes of a widget stack during transition phases.
     *
     * This function is called when a widget stack begins or ends a transition. During the transition,
     * it manages input suspension and resumption for the local player to ensure a smooth user experience.
     *
     * @param WidgetStack A pointer to the activatable widget container base that is transitioning.
     * @param bIsTransitioning Indicates whether the widget stack is currently in transition.
     *                         True for beginning a transition, False for ending a transition.
     */
    void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase *Widget, bool bIsTransitioning);

  private:
    bool bIsDormant = false;

    TArray<FName> SuspendInputTokens;

    UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
