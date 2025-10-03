// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GameUIPolicy.generated.h"

class UInputMappingContext;
class URPGLocalPlayer;
class UPrimaryGameLayout;
class UGameUIManagerSubsystem;

/**
 * @enum ELocalMultiplayerInteractionMode
 * @brief Enum representing different interaction modes for local multiplayer gameplay.
 *
 * This enum defines how the screen/viewports are shared and managed between players during local multiplayer
 * gameplay. It provides options to determine whether a single player has full control of the viewport or if
 * multiple players interact concurrently.
 *
 * The modes in this enumeration allow for implementing gameplay mechanics and visual arrangements
 * suited for different multiplayer experiences.
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
    /**
     * Fullscreen viewport for the primary player only, regardless of the other player's existence
     */
    PrimaryOnly,

    /**
     * Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
     */
    SingleToggle,

    /**
     * Viewports displayed simultaneously for both players
     */
    Simultaneous
};

/**
 * @struct FRootViewportLayoutInfo
 * @brief Represents layout information for a player's root viewport in the game UI.
 *
 * This structure encapsulates data related to the viewport and primary game layout associated with a specific local
 * player. It is utilized to manage and track the relationship between players and their respective UI elements in the
 * game.
 *
 * The structure includes the player reference, the root game layout instance, and a flag indicating whether
 * the layout has been added to the viewport.
 */
USTRUCT()
struct FRootViewportLayoutInfo
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    TObjectPtr<ULocalPlayer> LocalPlayer;

    UPROPERTY(Transient)
    TObjectPtr<UPrimaryGameLayout> RootLayout;

    UPROPERTY(Transient)
    bool bAddedToViewport = false;

    FRootViewportLayoutInfo() = default;

    FRootViewportLayoutInfo(ULocalPlayer *InLocalPlayer, UPrimaryGameLayout *InRootLayout, bool bIsInViewport)
        : LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bAddedToViewport(bIsInViewport)
    {
    }

    bool operator==(const ULocalPlayer *OtherLocalPlayer) const
    {
        return LocalPlayer == OtherLocalPlayer;
    }
};

/**
 * @class UGameUIPolicy
 * @brief A base class for defining policies related to Game User Interface (UI) behavior and management.
 *
 * This class serves as a framework to manage and define the policies and behaviors associated with
 * UI elements in a game. It deals with aspects like UI layers, visibility, input focus, navigation,
 * and interaction. UGameUIPolicy can be extended to implement game-specific UI management logic.
 *
 * This class helps centralize and standardize how the UI is handled in a game environment,
 * providing a structure for managing complex UI hierarchies, transitions, and state changes
 * efficiently.
 *
 * The class is generally utilized as part of the UI framework in a game engine, ensuring that
 * UIs are displayed and function per the game's design requirements.
 */
UCLASS(Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class INTERACTIVEUI_API UGameUIPolicy : public UObject
{
    GENERATED_BODY()

  public:
    UGameUIPolicy() = default;

    template <std::derived_from<UGameUIPolicy> PolicyClass = UGameUIPolicy>
    static PolicyClass *GetInstance(const UObject *WorldContextObject)
    {
        return Cast<PolicyClass>(GetInstance(WorldContextObject));
    }

    UFUNCTION(meta = (WorldContext = "WorldContextObject", ScriptMethod))
    static UGameUIPolicy *GetInstance(const UObject *WorldContextObject);

    UWorld *GetWorld() const override;

    UGameUIManagerSubsystem *GetOwner() const;

    UPrimaryGameLayout *GetRootLayout(const ULocalPlayer *Player) const;

    ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const
    {
        return LocalMultiplayerInteractionMode;
    }

    void RequestPrimaryControl(UPrimaryGameLayout *Layout);

  protected:
    UFUNCTION(meta = (ScriptMethod))
    void AddLayoutToViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    UFUNCTION(meta = (ScriptMethod))
    void RemoveLayoutFromViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutAddedToViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutRemovedFromViewport(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutReleased(ULocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    UFUNCTION(meta = (ScriptMethod))
    void CreateLayoutWidget(ULocalPlayer *LocalPlayer);

    UFUNCTION(meta = (ScriptMethod))
    TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass() const;

  private:
    void NotifyPlayerAdded(ULocalPlayer *LocalPlayer);
    void NotifyPlayerRemoved(ULocalPlayer *LocalPlayer);
    void NotifyPlayerDestroyed(ULocalPlayer *LocalPlayer);

    friend class UGameUIManagerSubsystem;

    ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

    UPROPERTY(Transient)
    TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
