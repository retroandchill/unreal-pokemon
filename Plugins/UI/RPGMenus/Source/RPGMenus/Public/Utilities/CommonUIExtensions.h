// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonUIExtensions.generated.h"

class UCommonActivatableWidget;
/**
 *
 */
UCLASS()
class RPGMENUS_API UCommonUIExtensions : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
    static FName SuspendInputForPlayer(APlayerController *PlayerController, FName SuspendReason);

    static FName SuspendInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendReason);

    UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
    static void ResumeInputForPlayer(APlayerController *PlayerController, FName SuspendToken);

    static void ResumeInputForPlayer(const ULocalPlayer *LocalPlayer, FName SuspendToken);

  private:
    static int32 InputSuspensions;
};
