// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonUIExtensions.generated.h"

class UCommonActivatableWidget;
/**
 *
 */
UCLASS()
class INTERACTIVEUI_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static ECommonInputType GetOwningPlayerInputType(const UUserWidget *WidgetContextObject);

    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static bool IsOwningPlayerUsingTouch(const UUserWidget *WidgetContextObject);

    UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (WorldContext = "WidgetContextObject", ExtensionMethod))
    static bool IsOwningPlayerUsingGamepad(const UUserWidget *WidgetContextObject);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions",
              meta = (DeterminesOutputType = "WidgetClass", DynamicOutputParam = "ReturnValue", ExtensionMethod))
    static UCommonActivatableWidget *PushContentToLayer(
        const ULocalPlayer *LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
        UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void PushStreamedContentToLayer(
        const ULocalPlayer *LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
        UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void PopContentFromLayer(UCommonActivatableWidget *ActivatableWidget);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static ULocalPlayer *GetLocalPlayerFromController(APlayerController *PlayerController);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static FName SuspendInputForPlayer(APlayerController *PlayerController, FName SuspendReason);

    static FName SuspendInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendReason);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions", meta = (ExtensionMethod))
    static void ResumeInputForPlayer(APlayerController *PlayerController, FName SuspendToken);

    static void ResumeInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendToken);

  private:
    static int32 InputSuspensions;
};
