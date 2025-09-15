// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GameUIPolicy.generated.h"

class URPGLocalPlayer;
class UPrimaryGameLayout;
class UGameUIManagerSubsystem;

/**
 *
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
    // Fullscreen viewport for the primary player only, regardless of the other player's existence
    PrimaryOnly,

    // Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
    SingleToggle,

    // Viewports displayed simultaneously for both players
    Simultaneous
};

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
class RPGMENUS_API UGameUIPolicy : public UObject
{
    GENERATED_BODY()

  public:
        UGameUIPolicy() = default;
    
    template <std::derived_from<UGameUIPolicy> PolicyClass = UGameUIPolicy>
    static PolicyClass* GetInstance(const UObject* WorldContextObject)
    {
        return Cast<PolicyClass>(GetInstance(WorldContextObject));
    }

    UFUNCTION(meta = (WorldContext = "WorldContextObject", ScriptMethod))
    static UGameUIPolicy* GetInstance(const UObject* WorldContextObject);

    UWorld* GetWorld() const override;

    UGameUIManagerSubsystem* GetOwner() const;
    
    UPrimaryGameLayout *GetRootLayout(const ULocalPlayer* Player) const;

    ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

    void RequestPrimaryControl(UPrimaryGameLayout* Layout);

protected:
    UFUNCTION(meta = (ScriptMethod))
    void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
    
    UFUNCTION(meta = (ScriptMethod))
    void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
    
    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
    
    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

    
    UFUNCTION(BlueprintNativeEvent, Category = "UI Policy")
    void OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

    UFUNCTION(meta = (ScriptMethod))
    void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
    
    UFUNCTION(meta = (ScriptMethod))
    TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass() const;

private:
    void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
    void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
    void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

    friend class UGameUIManagerSubsystem;
    
    ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

    UPROPERTY(Transient)
    TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};
