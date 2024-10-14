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

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UPauseMenuScreen *AddPauseMenuScreenToStack(const UObject *WorldContextObject);

  protected:
    void NativeConstruct() override;

  private:
    /**
     * The sound that plays when the menu is opened.
     */
    UPROPERTY(EditAnywhere, Category = Sound)
    TObjectPtr<USoundBase> OpenSound;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UPauseMenuScreen)