// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Extensions/Subsystems/SubsystemCollectionBaseRef.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GameUIManagerSubsystem.generated.h"

class URPGLocalPlayer;
class UGameUIPolicy;
class UInputMappingContext;

/**
 * The UGameUIManagerSubsystem is a subsystem designed to manage the game's UI.
 * It facilitates the initialization, deinitialization, and runtime management of UI policies
 * and provides mechanisms to notify changes in player states within the game.
 */
UCLASS(DisplayName = "Game UI Manager Subsystem", Config = Game)
class INTERACTIVEUI_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    /**
     * Default constructor for the game UI manager subsystem.
     * This initializes the subsystem with default behavior.
     *
     * @return A new instance of the UGameUIManagerSubsystem class.
     */
    UGameUIManagerSubsystem() = default;

    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;
    bool ShouldCreateSubsystem(UObject *Outer) const override;

    /**
     * Retrieves an instance of the UGameUIManagerSubsystem associated with a given world context object.
     * It provides access to the UI Manager Subsystem, allowing interaction and management
     * of game-specific UI policies and events.
     *
     * @param WorldContextObject The context object used to determine the world for which the subsystem is retrieved.
     * @return A pointer to the UGameUIManagerSubsystem instance for the specified world context.
     */
    UFUNCTION(meta = (ScriptMethod, WorldContext = WorldContextObject))
    static UGameUIManagerSubsystem *GetInstance(const UObject *WorldContextObject)
    {
        return UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UGameUIManagerSubsystem>();
    }

    /**
     * Retrieves the current UI policy being used by the game.
     * This method provides access to the active UI policy for managing game UI behavior.
     *
     * @return A pointer to the current UGameUIPolicy instance, or nullptr if no policy is active.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UGameUIPolicy *GetCurrentUIPolicy()
    {
        return CurrentPolicy;
    }

    /**
     * Retrieves the current UI policy being used by the game.
     * This method provides read-only access to the active UI policy for managing game UI behavior.
     *
     * @return A const pointer to the current UGameUIPolicy instance, or nullptr if no policy is active.
     */
    const UGameUIPolicy *GetCurrentUIPolicy() const
    {
        return CurrentPolicy;
    }

    /**
     * Notifies the game UI manager that a new player has been added.
     * This method enables the game to handle any necessary UI updates or adjustments
     * triggered by the addition of a new player.
     *
     * @param LocalPlayer The local player instance that was added to the game.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerAdded(ULocalPlayer *LocalPlayer);

    /**
     * Notifies the game UI manager that a player has been removed from the game.
     * This function allows the game to handle any necessary updates or adjustments
     * triggered by the removal of a player.
     *
     * @param LocalPlayer The local player instance that was removed from the game.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerRemoved(ULocalPlayer *LocalPlayer);

    /**
     * Notifies the game UI manager that a player has been destroyed in the game.
     * This function enables the game to handle necessary updates, such as releasing
     * resources or updating the UI, related to the destruction of a player.
     *
     * @param LocalPlayer The local player instance that was destroyed.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerDestroyed(ULocalPlayer *LocalPlayer);

  protected:
    /**
     * Switches the game UI manager subsystem to use a new UI policy.
     * This method updates the active UI policy that dictates the behavior and structure
     * of the game's user interface.
     *
     * @param NewPolicy The new UI policy to switch to. If nullptr, the current policy will be cleared.
     */
    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void SwitchToPolicy(UGameUIPolicy *NewPolicy);

    /**
     * Determines whether a subsystem instance should be created for the given object.
     * This method provides a blueprint-native, overridable function to define
     * custom logic for subsystem creation.
     *
     * @param Outer The outer UObject for which the subsystem creation is being considered.
     * @return True if the subsystem should be created, false otherwise.
     */
    UFUNCTION(BlueprintNativeEvent, meta = (ScriptName = "ShouldCreateSubsystem"), Category = "Managed Subsystems")
    bool K2_ShouldCreateSubsystem(UObject *Outer) const;

    /**
     * This method is a Blueprint implementable event designed to initialize a subsystem
     * with the provided subsystem collection. It serves as an entry point for custom
     * initialization logic that can be defined in Blueprints.
     *
     * @param Collection A reference to the subsystem collection used for subsystem initialization.
     */
    UFUNCTION(BlueprintImplementableEvent, meta = (ScriptName = "Initialize"), Category = "Managed Subsystems")
    void K2_Initialize(FSubsystemCollectionBaseRef Collection);

    /**
     * Blueprint-implementable event designed to deinitialize a subsystem.
     * This serves as an entry point for custom deinitialization logic
     * that can be defined within Blueprints. It provides a mechanism
     * through which Blueprint developers can handle any specific cleanup
     * or teardown operations for the subsystem.
     */
    UFUNCTION(BlueprintImplementableEvent, meta = (ScriptName = "Deinitialize"), Category = "Managed Subsystems")
    void K2_Deinitialize();

  private:
    TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;

    /**
     * Represents the currently active UI Policy within the UI Manager subsystem.
     * This policy dictates the behavior, structure, and configuration of the game's UI at runtime.
     * It is managed and updated internally to reflect changes in the application state.
     */
    UPROPERTY(Transient, BlueprintGetter = GetCurrentUIPolicy, Category = "UI Manager")
    TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

    /**
     * A configuration property used to define the default UI policy class for the game.
     * This class is dynamically loaded and serves as the blueprint for implementing
     * UI behavior and structure within the game.
     */
    UPROPERTY(Config, EditAnywhere)
    TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};