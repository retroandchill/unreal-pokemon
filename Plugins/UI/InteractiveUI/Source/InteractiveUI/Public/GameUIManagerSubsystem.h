// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Extensions/Subsystems/SubsystemCollectionBaseRef.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GameUIManagerSubsystem.generated.h"

class URPGLocalPlayer;
class UGameUIPolicy;
class UInputMappingContext;
class UScreen;
/**
 * Subsystem for managing the UI of the RPG Menus Plugin.
 */
UCLASS(DisplayName = "Game UI Manager Subsystem", Config = Game)
class INTERACTIVEUI_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    UGameUIManagerSubsystem() = default;

    void Initialize(FSubsystemCollectionBase& Collection) override;
    void Deinitialize() override;
    bool ShouldCreateSubsystem(UObject* Outer) const override;

    UFUNCTION(meta = (ScriptMethod, WorldContext = WorldContextObject))
    static UGameUIManagerSubsystem* GetInstance(const UObject* WorldContextObject)
    {
        return UGameplayStatics::GetGameInstance(WorldContextObject)->GetSubsystem<UGameUIManagerSubsystem>();
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }
    
    const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
    
    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);

    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);

    UFUNCTION(BlueprintNativeEvent, Category = "UI Manager")
    void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

protected:
    UFUNCTION(BlueprintCallable, Category = "UI Manager")
    void SwitchToPolicy(UGameUIPolicy* NewPolicy);

    UFUNCTION(BlueprintNativeEvent, meta = (ScriptName = "ShouldCreateSubsystem"), Category = "Managed Subsystems")
    bool K2_ShouldCreateSubsystem(UObject* Outer) const;

    UFUNCTION(BlueprintImplementableEvent, meta = (ScriptName = "Initialize"), Category = "Managed Subsystems")
    void K2_Initialize(FSubsystemCollectionBaseRef Collection);

    UFUNCTION(BlueprintImplementableEvent, meta = (ScriptName = "Deinitialize"), Category = "Managed Subsystems")
    void K2_Deinitialize();

private:
    TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
    
    UPROPERTY(Transient, BlueprintGetter = GetCurrentUIPolicy, Category = "UI Manager")
    TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

    UPROPERTY(Config, EditAnywhere)
    TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};

namespace RPG::Menus
{
    struct FGetCurrentUIPolicyInvoker
    {
        template <typename T>
            requires std::same_as<std::remove_const_t<T>, UGameUIManagerSubsystem>
        constexpr auto operator()(T &Manager) const
        {
            return Manager.GetCurrentUIPolicy();
        }

        template <typename T>
            requires std::same_as<std::remove_const_t<T>, UGameUIManagerSubsystem>
        constexpr auto operator()(T *Manager) const
        {
            return Manager->GetCurrentUIPolicy();
        }
    };

    constexpr FGetCurrentUIPolicyInvoker GetCurrentUIPolicy;
} // namespace RPG::Menus