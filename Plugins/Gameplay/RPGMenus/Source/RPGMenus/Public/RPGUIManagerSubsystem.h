// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "RPGUIManagerSubsystem.generated.h"

class URPGLocalPlayer;
class UGameUIPolicy;
class UInputMappingContext;
class UScreen;
/**
 * Subsystem for managing the UI of the RPG Menus Plugin.
 */
UCLASS(DisplayName = "RPG UI Manager Subsystem", Config = Game)
class RPGMENUS_API URPGUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;
    bool ShouldCreateSubsystem(UObject *Outer) const override;

    static URPGUIManagerSubsystem &Get(const UObject *WorldContext);

    const UGameUIPolicy *GetCurrentUIPolicy() const
    {
        return CurrentPolicy;
    }
    UGameUIPolicy *GetCurrentUIPolicy()
    {
        return CurrentPolicy;
    }

    void NotifyPlayerAdded(URPGLocalPlayer *LocalPlayer);
    void NotifyPlayerRemoved(URPGLocalPlayer *LocalPlayer);
    void NotifyPlayerDestroyed(URPGLocalPlayer *LocalPlayer);

    UFUNCTION(BlueprintPure, Category = Screens)
    UScreen *GetTopScreenOfStack() const;

    UFUNCTION(BlueprintPure, Category = Screens)
    UScreen *GetTopScreenOfOverlay() const;

    void OnScreenActivated(UScreen *Screen);

    void OnScreenDeactivated(UScreen *Screen);

  protected:
    void SwitchToPolicy(UGameUIPolicy *InPolicy);

  private:
    UPROPERTY(Transient)
    TObjectPtr<UGameUIPolicy> CurrentPolicy;

    UPROPERTY(Config, EditAnywhere)
    TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;

    UPROPERTY()
    TObjectPtr<UInputMappingContext> MenuMappingContext;

    uint32 ActiveScreenCount = 0;
};

namespace RPG::Menus
{
    struct FGetCurrentUIPolicyInvoker
    {
        template <typename T>
            requires std::same_as<std::remove_const_t<T>, URPGUIManagerSubsystem>
        constexpr auto operator()(T &Manager) const
        {
            return Manager.GetCurrentUIPolicy();
        }

        template <typename T>
            requires std::same_as<std::remove_const_t<T>, URPGUIManagerSubsystem>
        constexpr auto operator()(T *Manager) const
        {
            return Manager->GetCurrentUIPolicy();
        }
    };

    constexpr FGetCurrentUIPolicyInvoker GetCurrentUIPolicy;
} // namespace RPG::Menus