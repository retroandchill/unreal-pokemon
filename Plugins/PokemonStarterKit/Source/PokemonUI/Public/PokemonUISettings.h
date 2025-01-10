// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonUISettings.generated.h"

USTRUCT(BlueprintType)
struct POKEMONUI_API FItemMessages {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, DisplayName = "Already Holding Item (Vowel)")
    FText AlreadyHoldingItemVowel = NSLOCTEXT("PokemonUI", "HoldingItemVowel", "{Pkmn} is already holding an {Item}.");

    UPROPERTY(EditAnywhere, DisplayName = "Already Holding Item (Consonant)")
    FText AlreadyHoldingItemConsonant =
        NSLOCTEXT("PokemonUI", "HoldingItemConsonant", "{Pkmn} is already holding a {Item}.");

    UPROPERTY(EditAnywhere)
    FText SwitchItemPrompt = NSLOCTEXT("PokemonUI", "SwitchItemsPrompt", "Would you like to switch the two items?");

    UPROPERTY(EditAnywhere)
    FText SwappedItemsMessage =
        NSLOCTEXT("PokemonUI", "SwappedItemsMessage", "Took the {Original} from {Pkmn} and gave it the {New}.");

    UPROPERTY(EditAnywhere)
    FText NowHoldingMessage = NSLOCTEXT("PokemonUI", "NowHoldingMessage", "{Pkmn} is now holding the {Item}.");
};

/**
 * Settings for all the Pokémon UI related windows.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon UI")
class POKEMONUI_API UPokemonUISettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The screen displayed when showing a messages
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "TextDisplayScreen"))
    FSoftClassPath TextScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "PokemonSelectScreen"))
    FSoftClassPath PartyScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "BagScreen"))
    FSoftClassPath BagScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "MoveForgetScreen"))
    FSoftClassPath MoveForgetScreenClass;

    /**
     * The screen displayed when prompting to save
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "SaveScreen"))
    FSoftClassPath SaveScreenClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Messages)
    FItemMessages ItemMessages;
};
