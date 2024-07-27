// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Screens/Screen.h"

#include "PauseMenuScreen.generated.h"

class UCommand;
class UCommandWindow;
/**
 * Basic Pokémon pause menu screen
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UPauseMenuScreen : public UScreen {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  private:
    /**
     * The sound that plays when the menu is opened.
     */
    UPROPERTY(EditAnywhere, Category = Sound)
    TObjectPtr<USoundBase> OpenSound;
};
