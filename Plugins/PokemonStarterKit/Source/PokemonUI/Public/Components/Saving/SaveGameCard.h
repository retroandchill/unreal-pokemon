// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SaveGameCard.generated.h"

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

    UFUNCTION(BlueprintImplementableEvent, Category = Layout)
    void SlotPokemonIcon(UImage *Icon);

private:
    void UpdateTimeLabels();

    UImage* CreatePokemonIcon(const TScriptInterface<IPokemon>& Pokemon);
    
    UFUNCTION()
    void UpdatePlaytimeText(float Playtime);
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> DateText;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> TimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> LocationText;

    UPROPERTY()
    TArray<TObjectPtr<UImage>> Icons;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> GymBadgeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokedexText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PlaytimeText;

    /**
     * The format used for formatting the date
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FString DateFormat = TEXT("%m/%d/%Y");

    FTimerHandle TimeUpdateTimer;
};
