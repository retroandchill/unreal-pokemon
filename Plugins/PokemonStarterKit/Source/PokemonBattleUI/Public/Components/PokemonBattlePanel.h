// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "PokemonUI/Public/Components/PokemonInfoWidget.h"
#include "Text/TextColor.h"

#include "PokemonBattlePanel.generated.h"

class IBattler;
class UProgressBar;
class UDisplayText;
/**
 * The panel that displays the battle information about a Pokémon.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonBattlePanel : public UUserWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    void SetBattler(const TScriptInterface<IBattler>& Battler);
    
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    virtual void Refresh();

protected:
    const TScriptInterface<IBattler>& GetCurrentBattler() const;
    
private:
    
    /**
     * The widget to display the Pokémon's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonName;

    /**
     * The widget to display the Pokémon's gender
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonGender;

    /**
     * The widget to display the Pokémon's level
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonLevel;

    /**
     * The widget that display's the HP percentage
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;

    /**
     * The battler this widget displays the information for
     */
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    /**
     * The color settings for the gender text widget
     */
    UPROPERTY(EditAnywhere, Category = "Visuals|Text")
    TMap<EPokemonGender, FTextColor> GenderTextColors;

};
