// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PokemonMessageSettings.generated.h"

USTRUCT(BlueprintType)
struct POKEMONUI_API FItemMessages {
    GENERATED_BODY()

    
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon Standard Messages")
class POKEMONUI_API UPokemonMessageSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, DisplayName = "Already Holding Item (Vowel)", Category = HoldItems)
    FText AlreadyHoldingItemVowel = NSLOCTEXT("PokemonUI", "HoldingItemVowel", "{Pkmn} is already holding an {Item}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, DisplayName = "Already Holding Item (Consonant)", Category = HoldItems)
    FText AlreadyHoldingItemConsonant =
        NSLOCTEXT("PokemonUI", "HoldingItemConsonant", "{Pkmn} is already holding a {Item}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = HoldItems)
    FText SwitchItemPrompt = NSLOCTEXT("PokemonUI", "SwitchItemsPrompt", "Would you like to switch the two items?");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = HoldItems)
    FText SwappedItemsMessage =
        NSLOCTEXT("PokemonUI", "SwappedItemsMessage", "Took the {Original} from {Pkmn} and gave it the {New}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = HoldItems)
    FText NowHoldingMessage = NSLOCTEXT("PokemonUI", "NowHoldingMessage", "{Pkmn} is now holding the {Item}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = HoldItems)
    FText TookItemMessage = NSLOCTEXT("PokemonUI", "TookItemMessage", "Received the {Item} from {Pkmn}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = HoldItems)
    FText CannotTakeItemMessage = NSLOCTEXT("PokemonUI", "CannotTakeItemMessage", "The Bag is full. The Pokémon's item could not be removed.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Moves)
    FText LearnedMoveMessage = NSLOCTEXT("PokemonUI", "LearnedMoveMessage", "{Pkmn} learned {Move}!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Moves)
    FText TryingToLearnMessage = NSLOCTEXT("PokemonUI", "TryingToLearnMessage", "{Pkmn} wants to learn {Move}, but it already knows {Count} moves.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Moves)
    FText ForgetMovePrompt = NSLOCTEXT("PokemonUI", "ForgetMovePrompt", "Should {Pkmn} forget a move to learn {Move}?");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Moves)
    FText MoveNotLearnedMessage = NSLOCTEXT("PokemonUI", "LearnedMoveMessage", "{Pkmn} did not learn {Move}.");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Leveling)
    FText GrewToLevelMessage = NSLOCTEXT("PokemonUI", "GrewToLevelMessage", "{Pkmn} grew to level {Level}!");
};
