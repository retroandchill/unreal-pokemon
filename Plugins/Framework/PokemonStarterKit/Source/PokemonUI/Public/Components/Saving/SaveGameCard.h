// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"

#include "SaveGameCard.generated.h"

class UCommonTextBlock;
class UPaperFlipbookUserWidget;
class IPokemon;
class UImage;
class UDisplayText;
/**
 * Fills out the information inside the card for the save screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USaveGameCard : public UCommonUserWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SetContents();

  private:
    void UpdateTimeLabels();

    UFUNCTION()
    void UpdatePlaytimeText(float Playtime);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> DateText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> TimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> PlaytimeText;

    /**
     * The format used for formatting the date
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FString DateFormat = TEXT("%m/%d/%Y");

    FTimerHandle TimeUpdateTimer;
};
