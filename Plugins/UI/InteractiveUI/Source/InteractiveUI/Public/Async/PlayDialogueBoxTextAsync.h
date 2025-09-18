// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAsyncActionBase.h"

#include "PlayDialogueBoxTextAsync.generated.h"

class UInputAction;
class UDialogueBox;

/**
 *
 */
UCLASS(meta = (InternalType))
class INTERACTIVEUI_API UPlayDialogueBoxTextAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    void PlayDialogueBoxText(UDialogueBox *DialogueBox, const FText &Text);

  private:
    void OnAsyncLoadComplete();

    UPROPERTY()
    TObjectPtr<UDialogueBox> Widget;

    FDelegateHandle DelegateHandle;
};
