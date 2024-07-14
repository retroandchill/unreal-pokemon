// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonInfoWidget.h"
#include "PokemonInfoPage.h"

#include "PokemonStatRow.generated.h"

class UCommonNumericTextBlock;
class UCommonRichTextBlock;
class IStatEntry;

/**
 * Represents a row in the stat display screen.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonStatRow : public UPokemonInfoWidget {
    GENERATED_BODY()

  public:
    /**
     * Set the stat that this row displays.
     * @param NewDisplayedStat The actual stat that is being displayed by this element.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Stats")
    void SetDisplayedStat(FName NewDisplayedStat);

  protected:
    void Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

    /**
     * Get the text widget for the value of the stat.
     * @return The text widget for the value of the stat.
     */
    UFUNCTION(BlueprintPure, Category = "Stats")
    UCommonTextBlock *GetStatValue() const;

    /**
     * Set the value text of the given stat information
     * @param Pokemon The Pokémon that owns the stat
     * @param StatInfo The information about the stat
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Stats")
    void SetStatValueText(const TScriptInterface<IPokemon> &Pokemon, const TScriptInterface<IStatEntry> &StatInfo);

  private:
    /**
     * The actual stat that is being displayed by this element.
     */
    UPROPERTY(EditAnywhere, BlueprintSetter = SetDisplayedStat, Category = "Stats",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName DisplayedStat;

    /**
     * The text format for a stat that is boosted.
     */
    UPROPERTY(EditAnywhere, Category = "Stat")
    FText BoostedFormat = FText::FromStringView(TEXT("<Boosted>{0}</>"));

    /**
     * The text format for a stat that is decreased.
     */
    UPROPERTY(EditAnywhere, Category = "Stat")
    FText DecreasedFormat = FText::FromStringView(TEXT("<Decreased>{0}</>"));

    /**
     * The text label for the name of the stat.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonRichTextBlock> StatLabel;

    /**
     * The text widget for the value of the stat.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> StatValue;
};
