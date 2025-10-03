// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAsyncActionBase.h"

#include "PlayDialogueBoxTextAsync.generated.h"

class UInputAction;
class UDialogueBox;

/**
 * UPlayDialogueBoxTextAsync is an asynchronous action for managing and playing
 * text in a DialogueBox widget. It allows interaction with a UDialogueBox instance,
 * enabling text playback with asynchronous handling of completion.
 */
UCLASS(meta = (InternalType))
class INTERACTIVEUI_API UPlayDialogueBoxTextAsync : public UCSAsyncActionBase
{
    GENERATED_BODY()

  public:
    /**
     * PlayDialogueBoxText is designed to control the sequential playback of a line
     * of dialogue text through a specified UDialogueBox widget. This method plays
     * the provided text in conjunction with handling events such as detecting when
     * a given line has finished playing.
     *
     * @param DialogueBox A pointer to the UDialogueBox widget responsible for displaying
     *        and playing the dialogue text.
     * @param Text The FText to be played, representing the dialogue line intended
     *        for playback in the designated dialogue box.
     */
    UFUNCTION(meta = (ScriptMethod))
    void PlayDialogueBoxText(UDialogueBox *DialogueBox, const FText &Text);

  private:
    /**
     * OnAsyncLoadComplete is a private callback method triggered when a line of dialogue
     * has finished playing in the associated UDialogueBox instance. This method handles
     * cleanup of event bindings and invokes the relevant managed callback to signal
     * the completion of the asynchronous operation.
     */
    void OnAsyncLoadComplete();

    UPROPERTY()
    TObjectPtr<UDialogueBox> Widget;

    FDelegateHandle DelegateHandle;
};
