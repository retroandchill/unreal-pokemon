// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "UE5Coro.h"
#include "Utilities/CommonUIExtensions.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "PrimaryGameLayout.generated.h"

/**
 *
 */
UCLASS()
class RPGMENUS_API UPrimaryGameLayout : public UCommonUserWidget
{
    GENERATED_BODY()

  public:
    static UPrimaryGameLayout *Get(const UObject *WorldContextObject);
    static UPrimaryGameLayout *Get(const APlayerController *PlayerController);
    static UPrimaryGameLayout *Get(ULocalPlayer *LocalPlayer);

    bool IsDormant() const
    {
        return bIsDormant;
    }

    void SetIsDormant(bool Dormant);

    template <std::derived_from<UCommonActivatableWidget> T = UCommonActivatableWidget>
    UE5Coro::TCoroutine<T *> PushWidgetToLayerStackAsync(FGameplayTag LayerName,
                                                         TSoftClassPtr<T> ActivatableWidgetClass,
                                                         bool bSuspendUntilInputComplete, FForceLatentCoroutine = {})
    {
        static const FName PushingToWidgetLayer = "PushingWidgetToLayer";
        const FName SuspendInputToken =
            bSuspendUntilInputComplete
                ? UCommonUIExtensions::SuspendInputForPlayer(GetOwningPlayer(), PushingToWidgetLayer)
                : NAME_None;

        UE5Coro::FOnCoroutineCanceled OnCanceled([this, SuspendInputToken] {
            UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
        });
        auto Class = co_await UE5Coro::Latent::AsyncLoadClass(std::move(ActivatableWidgetClass));

        UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

        auto Widget = PushWidgetToLayerStack<T>(LayerName, Class);
        co_return Widget;
    }

    template <std::derived_from<UCommonActivatableWidget> T = UCommonActivatableWidget>
    T *PushWidgetToLayerStack(FGameplayTag LayerName, TSubclassOf<T> WidgetClass)
    {
        auto Layer = GetLayerWidget(LayerName);
        if (Layer == nullptr)
        {
            return nullptr;
        }

        return Layer->AddWidget<T>(WidgetClass);
    }

    void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget *ActivatableWidget);

    UCommonActivatableWidgetContainerBase *GetLayerWidget(FGameplayTag LayerName);

  protected:
    UFUNCTION(BlueprintCallable, Category = "Layer")
    void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag,
                       UCommonActivatableWidgetContainerBase *LayerWidget);

    virtual void OnIsDormantChanged();

    void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase *Widget, bool bIsTransitioning);

  private:
    bool bIsDormant = false;

    TArray<FName> SuspendInputTokens;

    UPROPERTY(Transient, meta = (Categories = "UI|Layer"))
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
