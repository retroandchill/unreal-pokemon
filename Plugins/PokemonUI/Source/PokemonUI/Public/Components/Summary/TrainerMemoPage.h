// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SummaryScreenPage.h"
#include "Pokemon/TrainerMemo/ObtainMethod.h"
#include "Screens/PokemonSummaryScreen.h"

#include "TrainerMemoPage.generated.h"

class UDisplayText;

/**
 * Struct that contains a list of characteristics.
 */
USTRUCT(BlueprintType)
struct FCharacteristicList {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
    TArray<FText> Characteristics;

    explicit FCharacteristicList(TArray<FText> &&Characteristics);
};

/**
 * The page used to outline the trainer memo on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API UTrainerMemoPage : public USummaryScreenPage {
    GENERATED_BODY()

  public:
    /**
     * Initialize the default object.
     * @param Initializer The intializer passed on construction
     */
    explicit UTrainerMemoPage(const FObjectInitializer &Initializer);

  protected:
    void RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

    /**
     * Should the game show the Pokémon's nature?
     * @return Will the nature be displayed?
     */
    UFUNCTION(BlueprintNativeEvent, Caegory = "Memo|Formatting")
    bool ShowNature() const;

  private:
    FText FormatDate(const FDateTime &DateTime) const;

    FText FormatLocation(FText Location) const;

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
    FText NatureLineFormat;

    /**
     * The format used for the date obtained line.
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FString DateLineFormat;

    /**
     * The name used when the location obtained is unknown.
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText UnknownObtainLocation;

    /**
     * The format for the location the Pokémon was obtained at
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText LocationFormatting;

    /**
     * The text used for the level the Pokémon was obtained in
     * <p>Use {Level} for the level.</p>
     */
    UPROPERTY(EditAnywhere, EditFixedSize, Category = "Memo|Formatting")
    TMap<EObtainMethod, FText> ObtainLocationFormats;

    /**
     * The text used for saying the egg hatched
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FText EggHatchedText;

    /**
     * The order of the stats used for tiebreakers
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting|Characteristic",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    TArray<FName> StatsOrder;

    /**
     * The order of the stats used for tiebreakers
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting|Characteristic",
              meta = (GetKeyOptions = "PokemonData.StatHelper.GetMainStatNames"))
    TMap<FName, FCharacteristicList> Characteristics;
};
