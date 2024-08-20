// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Pokemon/TrainerMemo/ObtainMethod.h"
#include "Screens/PokemonSummaryScreen.h"

#include "TrainerMemoPage.generated.h"

#define MEMO_TEXT(Key, Text) NSLOCTEXT("UTrainerMemoPage", Key, Text)

class UDisplayText;

/**
 * Struct that contains a list of characteristics.
 */
USTRUCT(BlueprintType)
struct FCharacteristicList {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
    TArray<FText> Characteristics;

    FCharacteristicList();

    explicit FCharacteristicList(TArray<FText> &&Characteristics);
};

/**
 * The page used to outline the trainer memo on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API UTrainerMemoPage : public USummaryScreenPage {
    GENERATED_BODY()

  protected:
    void RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

    /**
     * Should the game show the Pokémon's nature?
     * @return Will the nature be displayed?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Memo|Formatting")
    bool ShowNature() const;

  private:
    FText FormatDate(const FDateTime &DateTime) const;

    FText FormatLocation(const FText& Location) const;

    /**
     * The text box used to display the information memo.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> MemoBlock;

    /**
     * The format used for the nature line.
     * <p>Use {Nature} for the nature.</p>
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText NatureLineFormat = MEMO_TEXT("NatureLineFormat", "<Blue>{Nature}</> nature.");

    /**
     * The format used for the date obtained line.
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FString DateLineFormat = TEXT("%b %e, %Y");

    /**
     * The name used when the location obtained is unknown.
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText UnknownObtainLocation = MEMO_TEXT("UnknownObtainLocation", "Faraway place");

    /**
     * The format for the location the Pokémon was obtained at
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText LocationFormatting = MEMO_TEXT("LocationFormatting", "<Red>{Location}</>");

    /**
     * The text used for the level the Pokémon was obtained in
     * <p>Use {Level} for the level.</p>
     */
    UPROPERTY(EditAnywhere, EditFixedSize, Category = "Memo|Formatting")
    TMap<EObtainMethod, FText> ObtainLocationFormats = {
        {EObtainMethod::Default, MEMO_TEXT("ObtainLocationDefault", "Met at Lv. {Level}.")},
        {EObtainMethod::Egg, MEMO_TEXT("ObtainLocationEgg", "Egg received.")},
        {EObtainMethod::Trade, MEMO_TEXT("ObtainLocationTrade", "Traded at Lv. {Level}.")},
        {EObtainMethod::FatefulEncounter,
         MEMO_TEXT("ObtainLocationFatefulEncounter", "Apparently had a fateful encounter at Lv. {Level}.")}};

    /**
     * The text used for saying the egg hatched
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText EggHatchedText = MEMO_TEXT("EggHatchedText", "Egg hatched.");

    /**
     * The order of the stats used for tiebreakers
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting|Characteristic",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    TArray<FName> StatsOrder = {"HP", "ATTACK", "DEFENSE", "SPEED", "SPECIAL_ATTACK", "SPECIAL_DEFENSE"};

    /**
     * The order of the stats used for tiebreakers
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting|Characteristic",
              meta = (GetKeyOptions = "PokemonData.StatHelper.GetMainStatNames"))
    TMap<FName, FCharacteristicList> Characteristics = {
        {"HP", FCharacteristicList({MEMO_TEXT("HP1", "Loves to eat."), MEMO_TEXT("HP2", "Takes plenty of siestas."),
                                    MEMO_TEXT("HP3", "Nods off a lot."), MEMO_TEXT("HP4", "Scatters things often."),
                                    MEMO_TEXT("HP5", "Likes to relax.")})},
        {"ATTACK", FCharacteristicList(
                       {MEMO_TEXT("ATTACK1", "Proud of its power."), MEMO_TEXT("ATTACK2", "Likes to thrash about."),
                        MEMO_TEXT("ATTACK3", "A little quick tempered."), MEMO_TEXT("ATTACK4", "Likes to fight."),
                        MEMO_TEXT("ATTACK5", "Quick tempered.")})},
        {"DEFENSE",
         FCharacteristicList({MEMO_TEXT("DEFENSE1", "Sturdy body."), MEMO_TEXT("DEFENSE2", "Capable of taking hits."),
                              MEMO_TEXT("DEFENSE3", "Highly persistent."), MEMO_TEXT("DEFENSE4", "Good endurance."),
                              MEMO_TEXT("DEFENSE5", "Good perseverance.")})},
        {"SPECIAL_ATTACK",
         FCharacteristicList(
             {MEMO_TEXT("SPECIAL_ATTACK1", "Highly curious."), MEMO_TEXT("SPECIAL_ATTACK2", "Mischievous."),
              MEMO_TEXT("SPECIAL_ATTACK3", "Thoroughly cunning."),
              MEMO_TEXT("SPECIAL_ATTACK4", "Often lost in thought."), MEMO_TEXT("SPECIAL_ATTACK5", "Very finicky.")})},
        {"SPECIAL_DEFENSE",
         FCharacteristicList(
             {MEMO_TEXT("SPECIAL_DEFENSE1", "Strong willed."), MEMO_TEXT("SPECIAL_DEFENSE2", "Somewhat vain."),
              MEMO_TEXT("SPECIAL_DEFENSE3", "Strongly defiant."), MEMO_TEXT("SPECIAL_DEFENSE4", "Hates to lose."),
              MEMO_TEXT("SPECIAL_DEFENSE5", "Somewhat stubborn.")})},
        {"SPEED",
         FCharacteristicList({MEMO_TEXT("SPEED1", "Likes to run."), MEMO_TEXT("SPEED2", "Alert to sounds."),
                              MEMO_TEXT("SPEED3", "Impetuous and silly."), MEMO_TEXT("SPEED4", "Somewhat of a clown."),
                              MEMO_TEXT("SPEED5", "Quick to flee.")})},
    };
};