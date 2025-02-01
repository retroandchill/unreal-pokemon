// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GameUIPolicy.generated.h"

class URPGLocalPlayer;
class UPrimaryGameLayout;
class URPGUIManagerSubsystem;

/**
 *
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8 {
    // Fullscreen viewport for the primary player only, regardless of the other player's existence
    PrimaryOnly,

    // Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
    SingleToggle,

    // Viewports displayed simultaneously for both players
    Simultaneous
};

USTRUCT()
struct FRootViewportLayoutInfo {
    GENERATED_BODY()

    UPROPERTY(Transient)
    TObjectPtr<ULocalPlayer> LocalPlayer;

    UPROPERTY(Transient)
    TObjectPtr<UPrimaryGameLayout> RootLayout;

    UPROPERTY(Transient)
    bool bAddedToViewport = false;

    FRootViewportLayoutInfo() = default;

    FRootViewportLayoutInfo(ULocalPlayer *InLocalPlayer, UPrimaryGameLayout *InRootLayout, bool bIsInViewport)
        : LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bAddedToViewport(bIsInViewport) {
    }

    bool operator==(const ULocalPlayer *OtherLocalPlayer) const {
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
UCLASS(Abstract, Blueprintable, Within = RPGUIManagerSubsystem)
class RPGMENUS_API UGameUIPolicy : public UObject {
    GENERATED_BODY()

  public:
    template <std::derived_from<UGameUIPolicy> T>
    static T *Get(const UObject *WorldContextObject) {
        if constexpr (std::same_as<T, UGameUIPolicy>) {
            return Get(WorldContextObject);
        } else {
            return Cast<T>(Get(WorldContextObject));
        }
    }

    static UGameUIPolicy *Get(const UObject *WorldContextObject);

    UWorld *GetWorld() const override;
    URPGUIManagerSubsystem *GetOwningUIManager() const;
    UPrimaryGameLayout *GetRootLayout(const URPGLocalPlayer *LocalPlayer) const;

    ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const {
        return LocalMultiplayerInteractionMode;
    }

    void RequestPrimaryControl(UPrimaryGameLayout *Layout);

  protected:
    void AddLayoutToViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);
    void RemoveLayoutFromViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    static void OnRootLayoutAddedToViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);
    static void OnRootLayoutRemovedFromViewport(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);
    static void OnRootLayoutReleased(URPGLocalPlayer *LocalPlayer, UPrimaryGameLayout *Layout);

    void CreateLayoutWidget(URPGLocalPlayer *LocalPlayer);
    TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(URPGLocalPlayer *LocalPlayer);

  private:
    ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

    friend class URPGUIManagerSubsystem;

    void NotifyPlayerAdded(URPGLocalPlayer *LocalPlayer);
    void NotifyPlayerRemoved(URPGLocalPlayer *LocalPlayer);
    void NotifyPlayerDestroyed(URPGLocalPlayer *LocalPlayer);

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

    UPROPERTY(Transient)
    TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
